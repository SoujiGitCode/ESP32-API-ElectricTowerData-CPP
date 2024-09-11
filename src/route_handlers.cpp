#include "route_handlers.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "config.h"
#include "functions.h"
#include "CellsCalibrationFactors.h"
#include <Ticker.h>

const char *username = "admin";
const char *loginPassword = "root";
const char *const ADMIN_PASSWORD = "3669225";
// pin que encendera definido en el
extern int ledPin;
extern Config config;                 // exporting config object
extern CellsCalibrationFactors Cells; // exporting config object
extern Ticker restartTimer;           // exporting restartTimer object

void handleLogin(AsyncWebServerRequest *request)
{
  if (request->hasParam("username", true) && request->hasParam("password", true))
  {
    String inputUsername = request->getParam("username", true)->value();
    String inputPassword = request->getParam("password", true)->value();
    if (inputUsername == username && inputPassword == loginPassword)
    {
      // Redirect to /dashboard
      request->redirect("/dashboard");
    }
    else
    {
      request->send(401, "text/html", "<html><body><h1>Unauthorized</h1></body></html>");
    }
  }
  else
  {
    request->send(400, "text/html", "<html><body><h1>Bad Request</h1></body></html>");
  }
}

void handleHome(AsyncWebServerRequest *request)
{
  if (!SPIFFS.exists("/index.html"))
  {
    request->send(404, "text/html", "File not found");
  }
  else
  {
    request->send(SPIFFS, "/index.html", "text/html");
  }
}

// API FUNCTIONS //

// Factory Reset
void handleFactoryReset(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_POST)
  {
    // Verificar si el parámetro admin_password está presente y es correcto
    if (!request->hasParam("admin_password", true) || request->getParam("admin_password", true)->value() != ADMIN_PASSWORD)
    {
      Serial.println("Invalid admin password");
      request->send(401, "application/json", "{\"status\": \"error\", \"message\": \"Invalid admin password\"}");
      return;
    }
    Serial.println("Admin password validated");

    // Llamar a la función que resetea a los valores de fábrica
    config.resetToFactoryDefaults();

    // Reiniciar el dispositivo en 2 segundos
    restartTimer.once(2, []()
                      { ESP.restart(); });

    // Responder con un mensaje de éxito
    request->send(200, "application/json", "{\"message\": \"Successfully reset to factory defaults\"}");
  }
  else
  {
    request->send(405, "application/json", "{\"message\": \"Method not allowed\"}");
  }
}

void handleSetWiFi(AsyncWebServerRequest *request)
{

  if (request->hasParam("ssid", true) && request->hasParam("password", true))
  {
    String ssid = request->getParam("ssid", true)->value();
    String password = request->getParam("password", true)->value();
    Serial.println("handleSetWifi params");
    Serial.println(ssid);
    Serial.println(password);
    // Comprueba la longitud del ssid y la contraseña
    if (!ssid.isEmpty() && password.length() >= 8 && password.length() <= 63)
    {

      Serial.println("params are okay!");
      config.setSsid(ssid.c_str());
      config.setPassword(password.c_str());
      request->send(200, "text/plain", "Configuración WiFi guardada");
      // Programa el dispositivo para reiniciar en 1 segundo
      restartTimer.once(0.1, []()
                        { ESP.restart(); });
    }
    else
    {
      request->send(400, "text/plain", "SSID vacío o longitud de contraseña inválida");
    }
  }
  else
  {
    request->send(400, "text/plain", "Parametros incorrectos");
  }
}

void handleSetCells(AsyncWebServerRequest *request)
{
  Serial.println("handleSetCells Called");

  // Verificar si el parámetro admin_password está presente y es correcto
  if (!request->hasParam("admin_password", true) || request->getParam("admin_password", true)->value() != ADMIN_PASSWORD)
  {
    request->send(401, "application/json", "{\"status\": \"error\", \"message\": \"Invalid admin password\"}");
    return;
  }
  Serial.println("Admin password validated");

  // Verificar si los valores de las celdas están presentes (ahora todos son obligatorios)
  AsyncWebParameter *p1 = request->getParam("first_cell", true);
  AsyncWebParameter *p2 = request->getParam("second_cell", true);
  AsyncWebParameter *p3 = request->getParam("third_cell", true);
  AsyncWebParameter *p4 = request->getParam("fourth_cell", true);
  AsyncWebParameter *r1 = request->getParam("first_read_1", true); // Obligatorio
  AsyncWebParameter *r2 = request->getParam("first_read_2", true); // Obligatorio
  AsyncWebParameter *r3 = request->getParam("first_read_3", true); // Obligatorio
  AsyncWebParameter *r4 = request->getParam("first_read_4", true); // Obligatorio

  // Verificar si los parámetros de las celdas están presentes
  if (p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL || r1 == NULL || r2 == NULL || r3 == NULL || r4 == NULL)
  {
    request->send(400, "application/json", "{\"status\": \"error\", \"message\": \"One or more required parameters not provided\"}");
    return;
  }

  // Convertir los valores de los parámetros a tipo String
  String first_cell = p1->value();
  String second_cell = p2->value();
  String third_cell = p3->value();
  String fourth_cell = p4->value();
  String first_read_1 = r1->value();
  String first_read_2 = r2->value();
  String first_read_3 = r3->value();
  String first_read_4 = r4->value();

  // Configurar los valores de las celdas
  Cells.set_first_cell(first_cell.toDouble());
  Cells.set_second_cell(second_cell.toDouble());
  Cells.set_third_cell(third_cell.toDouble());
  Cells.set_fourth_cell(fourth_cell.toDouble());

  // Configurar los valores de las primeras lecturas
  Cells.set_first_read_1(first_read_1.toDouble());
  Cells.set_first_read_2(first_read_2.toDouble());
  Cells.set_first_read_3(first_read_3.toDouble());
  Cells.set_first_read_4(first_read_4.toDouble());

  // Respuesta JSON con los valores de las celdas y las primeras lecturas
  String response = "{\"status\": \"success\", \"data\": {";
  response += "\"first_cell\": \"" + String(first_cell) + "\", ";
  response += "\"second_cell\": \"" + String(second_cell) + "\", ";
  response += "\"third_cell\": \"" + String(third_cell) + "\", ";
  response += "\"fourth_cell\": \"" + String(fourth_cell) + "\", ";
  response += "\"first_read_1\": \"" + String(Cells.get_first_read_1(), 2) + "\", ";
  response += "\"first_read_2\": \"" + String(Cells.get_first_read_2(), 2) + "\", ";
  response += "\"first_read_3\": \"" + String(Cells.get_first_read_3(), 2) + "\", ";
  response += "\"first_read_4\": \"" + String(Cells.get_first_read_4(), 2) + "\"";
  response += "}}";

  // Enviar respuesta
  request->send(200, "application/json", response);
}

void handleGetCells(AsyncWebServerRequest *request)
{
  // Obtener los valores de las celdas y las primeras lecturas
  String response = "{\"status\": \"success\", \"data\": {";
  response += "\"first_cell\": " + String(Cells.get_first_cell(), 2) + ", ";
  response += "\"second_cell\": " + String(Cells.get_second_cell(), 2) + ", ";
  response += "\"third_cell\": " + String(Cells.get_third_cell(), 2) + ", ";
  response += "\"fourth_cell\": " + String(Cells.get_fourth_cell(), 2) + ", ";
  response += "\"first_read_1\": " + String(Cells.get_first_read_1(), 2) + ", ";
  response += "\"first_read_2\": " + String(Cells.get_first_read_2(), 2) + ", ";
  response += "\"first_read_3\": " + String(Cells.get_first_read_3(), 2) + ", ";
  response += "\"first_read_4\": " + String(Cells.get_first_read_4(), 2);
  response += "}}";

  // Enviar la respuesta
  request->send(200, "application/json", response);
}

void handleGetTowerInfo(AsyncWebServerRequest *request)
{
  TowerInfo towerInfo = config.getTowerInfo();
  String response = serializeTowerInfo(towerInfo); // Utiliza la función serializeTowerInfo
  request->send(200, "application/json", response);
}

void handleSetTowerInfo(AsyncWebServerRequest *request)
{
  Serial.println("handleSetTowerInfo Called");

  // Verificar si el parámetro admin_password está presente y es correcto
  if (!request->hasParam("admin_password", true))
  {
    Serial.println("Admin password parameter missing");
    request->send(401, "application/json", "{\"status\": \"error\", \"message\": \"Admin password missing\"}");
    return;
  }
  else if (request->getParam("admin_password", true)->value() != ADMIN_PASSWORD)
  {
    Serial.println("Invalid admin password");
    request->send(401, "application/json", "{\"status\": \"error\", \"message\": \"Invalid admin password\"}");
    return;
  }

  Serial.println("Admin password validated");

  // Continuar con la verificación de otros parámetros y asignar valores predeterminados si no están presentes
  TowerInfo towerInfo;

  towerInfo.id = request->hasParam("id", true) ? request->getParam("id", true)->value() : "";
  towerInfo.name = request->hasParam("name", true) ? request->getParam("name", true)->value() : "";
  towerInfo.slang = request->hasParam("slang", true) ? request->getParam("slang", true)->value() : "";
  towerInfo.location = request->hasParam("location", true) ? request->getParam("location", true)->value() : "";
  towerInfo.priority = request->hasParam("priority", true) ? request->getParam("priority", true)->value().toInt() : 0;
  towerInfo.type = request->hasParam("type", true) ? request->getParam("type", true)->value() : "";
  towerInfo.loadcells_amount = request->hasParam("loadcells_amount", true) ? request->getParam("loadcells_amount", true)->value().toInt() : 0;

  // Guardar la información en las preferencias
  config.setTowerInfo(towerInfo);

  // Construir y enviar respuesta JSON
  String response = "{\"status\": \"success\", \"message\": \"Tower info updated successfully\", \"data\": ";
  response += serializeTowerInfo(towerInfo);
  response += "}";
  Serial.println("Response: " + response);

  request->send(200, "application/json", response);
}

void handleToggleLed(AsyncWebServerRequest *request)
{
  // Comprobar si se pasó el parámetro "led" en el form-data
  if (request->hasParam("led", true))
  {
    String ledState = request->getParam("led", true)->value();
    if (ledState == "true")
    {
      digitalWrite(ledPin, HIGH); // Enciende el LED
      request->send(200, "application/json", "{\"status\": \"success\", \"message\": \"LED encendido\"}");
    }
    else if (ledState == "false")
    {
      digitalWrite(ledPin, LOW); // Apaga el LED
      request->send(200, "application/json", "{\"status\": \"success\", \"message\": \"LED apagado\"}");
    }
    else
    {
      request->send(400, "application/json", "{\"status\": \"error\", \"message\": \"Parámetro 'led' inválido\"}");
    }
  }
  else
  {
    request->send(400, "application/json", "{\"status\": \"error\", \"message\": \"Falta el parámetro 'led'\"}");
  }
}

void configureRoutes(AsyncWebServer &server)
{
  server.on("/", HTTP_GET, handleHome);
  // API ROUTES //
  server.on("/api/factory-reset", HTTP_POST, handleFactoryReset);
  server.on("/api/set/wifi", HTTP_POST, handleSetWiFi);
  server.on("/api/set/cells", HTTP_POST, handleSetCells);
  server.on("/api/get/cells", HTTP_GET, handleGetCells);
  server.on("/api/set/tower-info", HTTP_POST, handleSetTowerInfo);
  server.on("/api/get/tower-info", HTTP_GET, handleGetTowerInfo);
  server.on("/api/led", HTTP_POST, handleToggleLed);

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
}
