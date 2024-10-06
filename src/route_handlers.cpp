#include "route_handlers.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "config.h"
#include "functions.h"
#include "CellsCalibrationFactors.h"
#include <Ticker.h>

extern const char *username;
extern const char *loginPassword;
extern const char *ADMIN_PASSWORD;
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

    // Preparar la respuesta
    String jsonResponse;

    if (inputUsername == username && inputPassword == loginPassword)
    {
      jsonResponse = "{\"status\": \"success\", \"message\": \"Login successful\"}";
    }
    else
    {
      jsonResponse = "{\"status\": \"error\", \"message\": \"Unauthorized\"}";
    }

    // Enviar la respuesta con encabezado CORS
    AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", jsonResponse);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
  }
  else
  {
    String jsonResponse = "{\"status\": \"error\", \"message\": \"Bad Request, missing username or password\"}";
    AsyncWebServerResponse *resp = request->beginResponse(400, "application/json", jsonResponse);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
  }
}

// void handleHome(AsyncWebServerRequest *request)
// {
//   if (!SPIFFS.exists("/index.html"))
//   {
//     request->send(404, "text/html", "File not found");
//   }
//   else
//   {
//     AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html", "text/html");
//     response->addHeader("Access-Control-Allow-Origin", "*");
//     request->send(response);
//   }
// }

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

// Factory Reset
void handleFactoryReset(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_POST)
  {
    if (!request->hasParam("admin_password", true) || request->getParam("admin_password", true)->value() != ADMIN_PASSWORD)
    {
      AsyncWebServerResponse *resp = request->beginResponse(401, "application/json", "{\"status\": \"error\", \"message\": \"Contraseña de Administrador Incorrecta\"}");
      resp->addHeader("Access-Control-Allow-Origin", "*");
      request->send(resp);
      return;
    }

    // Ejecutar el reset a valores de fábrica
    config.resetToFactoryDefaults();

    // Reiniciar el dispositivo después de 2 segundos
    restartTimer.once(2, []()
                      { ESP.restart(); });

    // Enviar respuesta de éxito con status y mensaje
    AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", "{\"status\": \"success\", \"message\": \"Dispositivo Reiniciado a valores de fabrica\"}");
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
  }
  else
  {
    // Responder con error si el método no es POST
    AsyncWebServerResponse *resp = request->beginResponse(405, "application/json", "{\"status\": \"error\", \"message\": \"Method not allowed\"}");
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
  }
}

void handleSetWiFi(AsyncWebServerRequest *request)
{
  if (request->hasParam("ssid", true) && request->hasParam("password", true))
  {
    String ssid = request->getParam("ssid", true)->value();
    String password = request->getParam("password", true)->value();

    if (!ssid.isEmpty() && password.length() >= 8 && password.length() <= 63)
    {
      config.setSsid(ssid.c_str());
      config.setPassword(password.c_str());

      String jsonResponse = "{\"status\": \"success\", \"message\": \"Configuración WiFi guardada exitosamente\"}";
      AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", jsonResponse);
      resp->addHeader("Access-Control-Allow-Origin", "*");
      request->send(resp);

      // Reiniciar el dispositivo en 0.1 segundos
      restartTimer.once(0.1, []()
                        { ESP.restart(); });
    }
    else
    {
      String jsonResponse = "{\"status\": \"error\", \"message\": \"SSID vacío o longitud de contraseña inválida\"}";
      AsyncWebServerResponse *resp = request->beginResponse(400, "application/json", jsonResponse);
      resp->addHeader("Access-Control-Allow-Origin", "*");
      request->send(resp);
    }
  }
  else
  {
    String jsonResponse = "{\"status\": \"error\", \"message\": \"Parámetros incorrectos\"}";
    AsyncWebServerResponse *resp = request->beginResponse(400, "application/json", jsonResponse);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
  }
}

void handleSetCells(AsyncWebServerRequest *request)
{
  // Verificar si el parámetro admin_password está presente y es correcto
  if (!request->hasParam("admin_password", true) || request->getParam("admin_password", true)->value() != ADMIN_PASSWORD)
  {
    String errorResponse = "{\"status\": \"error\", \"message\": \"Contraseña de administrador incorrecta\"}";
    AsyncWebServerResponse *resp = request->beginResponse(401, "application/json", errorResponse);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
    return;
  }

  // Verificar los parámetros
  AsyncWebParameter *p1 = request->getParam("first_cell", true);
  AsyncWebParameter *p2 = request->getParam("second_cell", true);
  AsyncWebParameter *p3 = request->getParam("third_cell", true);
  AsyncWebParameter *p4 = request->getParam("fourth_cell", true);
  AsyncWebParameter *r1 = request->getParam("first_read_1", true);
  AsyncWebParameter *r2 = request->getParam("first_read_2", true);
  AsyncWebParameter *r3 = request->getParam("first_read_3", true);
  AsyncWebParameter *r4 = request->getParam("first_read_4", true);

  // Validar si faltan parámetros
  if (p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL || r1 == NULL || r2 == NULL || r3 == NULL || r4 == NULL)
  {
    String errorResponse = "{\"status\": \"error\", \"message\": \"One or more required parameters not provided\"}";
    AsyncWebServerResponse *resp = request->beginResponse(400, "application/json", errorResponse);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
    return;
  }

  // Actualizar valores de las celdas
  Cells.set_first_cell(p1->value().toDouble());
  Cells.set_second_cell(p2->value().toDouble());
  Cells.set_third_cell(p3->value().toDouble());
  Cells.set_fourth_cell(p4->value().toDouble());
  Cells.set_first_read_1(r1->value().toDouble());
  Cells.set_first_read_2(r2->value().toDouble());
  Cells.set_first_read_3(r3->value().toDouble());
  Cells.set_first_read_4(r4->value().toDouble());

  // Construir respuesta de éxito
  String successResponse = "{\"status\": \"success\", \"message\": \"Cells configuration saved successfully\", \"data\": {";
  successResponse += "\"first_cell\": \"" + String(p1->value()) + "\", ";
  successResponse += "\"second_cell\": \"" + String(p2->value()) + "\", ";
  successResponse += "\"third_cell\": \"" + String(p3->value()) + "\", ";
  successResponse += "\"fourth_cell\": \"" + String(p4->value()) + "\", ";
  successResponse += "\"first_read_1\": \"" + String(Cells.get_first_read_1(), 2) + "\", ";
  successResponse += "\"first_read_2\": \"" + String(Cells.get_first_read_2(), 2) + "\", ";
  successResponse += "\"first_read_3\": \"" + String(Cells.get_first_read_3(), 2) + "\", ";
  successResponse += "\"first_read_4\": \"" + String(Cells.get_first_read_4(), 2) + "\"";
  successResponse += "}}";

  // Enviar respuesta de éxito
  AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", successResponse);
  resp->addHeader("Access-Control-Allow-Origin", "*");
  request->send(resp);
}

void handleGetCells(AsyncWebServerRequest *request)
{
  double first_cell = Cells.get_first_cell();
  double second_cell = Cells.get_second_cell();
  double third_cell = Cells.get_third_cell();
  double fourth_cell = Cells.get_fourth_cell();

  String response = "{\"status\": \"success\", \"data\": {";
  response += "\"first_cell\": \"" + String(first_cell, 2) + "\", ";
  response += "\"second_cell\": \"" + String(second_cell, 2) + "\", ";
  response += "\"third_cell\": \"" + String(third_cell, 2) + "\", ";
  response += "\"fourth_cell\": \"" + String(fourth_cell, 2) + "\", ";
  response += "\"first_read_1\": \"" + String(Cells.get_first_read_1(), 2) + "\", ";
  response += "\"first_read_2\": \"" + String(Cells.get_first_read_2(), 2) + "\", ";
  response += "\"first_read_3\": \"" + String(Cells.get_first_read_3(), 2) + "\", ";
  response += "\"first_read_4\": \"" + String(Cells.get_first_read_4(), 2) + "\"";
  response += "}}";

  AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", response);
  resp->addHeader("Access-Control-Allow-Origin", "*");
  request->send(resp);
}

void handleGetTowerInfo(AsyncWebServerRequest *request)
{
  TowerInfo towerInfo = config.getTowerInfo();

  String response = "{\"status\": \"success\", \"message\": \"Tower info retrieved successfully\", \"data\": {";
  response += "\"id\": \"" + towerInfo.id + "\", ";
  response += "\"name\": \"" + towerInfo.name + "\", ";
  response += "\"slang\": \"" + towerInfo.slang + "\", ";
  response += "\"location\": \"" + towerInfo.location + "\", ";
  response += "\"priority\": " + String(towerInfo.priority) + ", ";
  response += "\"type\": \"" + towerInfo.type + "\", ";
  response += "\"loadcells_amount\": " + String(towerInfo.loadcells_amount);
  response += "}}";

  AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", response);
  resp->addHeader("Access-Control-Allow-Origin", "*");
  request->send(resp);
}

void handleSetTowerInfo(AsyncWebServerRequest *request)
{
  if (!request->hasParam("admin_password", true))
  {
    AsyncWebServerResponse *resp = request->beginResponse(401, "application/json", "{\"status\": \"error\", \"message\": \"Falta contraseña de Administrador\"}");
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
    return;
  }
  else if (request->getParam("admin_password", true)->value() != ADMIN_PASSWORD)
  {
    AsyncWebServerResponse *resp = request->beginResponse(401, "application/json", "{\"status\": \"error\", \"message\": \"Contraseña de administrador incorrecta\"}");
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
    return;
  }

  TowerInfo towerInfo;

  towerInfo.id = request->hasParam("id", true) ? request->getParam("id", true)->value() : "";
  towerInfo.name = request->hasParam("name", true) ? request->getParam("name", true)->value() : "";
  towerInfo.slang = request->hasParam("slang", true) ? request->getParam("slang", true)->value() : "";
  towerInfo.location = request->hasParam("location", true) ? request->getParam("location", true)->value() : "";
  towerInfo.priority = request->hasParam("priority", true) ? request->getParam("priority", true)->value().toInt() : 0;
  towerInfo.type = request->hasParam("type", true) ? request->getParam("type", true)->value() : "";
  towerInfo.loadcells_amount = request->hasParam("loadcells_amount", true) ? request->getParam("loadcells_amount", true)->value().toInt() : 0;

  config.setTowerInfo(towerInfo);

  String response = "{\"status\": \"success\", \"message\": \"Tower info updated successfully\", \"data\": ";
  response += serializeTowerInfo(towerInfo);
  response += "}";

  AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", response);
  resp->addHeader("Access-Control-Allow-Origin", "*");
  request->send(resp);
}

void handleToggleLed(AsyncWebServerRequest *request)
{
  if (request->hasParam("led", true))
  {
    String ledState = request->getParam("led", true)->value();
    if (ledState == "true")
    {
      digitalWrite(ledPin, LOW);
      AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", "{\"status\": \"success\", \"message\": \"LED encendido\"}");
      resp->addHeader("Access-Control-Allow-Origin", "*");
      request->send(resp);
    }
    else if (ledState == "false")
    {
      digitalWrite(ledPin, HIGH);
      AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", "{\"status\": \"success\", \"message\": \"LED apagado\"}");
      resp->addHeader("Access-Control-Allow-Origin", "*");
      request->send(resp);
    }
    else
    {
      AsyncWebServerResponse *resp = request->beginResponse(400, "application/json", "{\"status\": \"error\", \"message\": \"Parámetro 'led' inválido\"}");
      resp->addHeader("Access-Control-Allow-Origin", "*");
      request->send(resp);
    }
  }
  else
  {
    AsyncWebServerResponse *resp = request->beginResponse(400, "application/json", "{\"status\": \"error\", \"message\": \"Falta el parámetro 'led'\"}");
    resp->addHeader("Access-Control-Allow-Origin", "*");
    request->send(resp);
  }
}

void configureRoutes(AsyncWebServer &server)
{
  server.onNotFound([](AsyncWebServerRequest *request)
                    {
    AsyncWebServerResponse *response = request->beginResponse(404, "application/json", "{\"message\":\"Not Found\"}");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response); });

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           {
  //   if (!SPIFFS.exists("/index.html")) {
  //     request->send(404, "text/html", "File not found");
  //   } else {
  //     AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html", "text/html");
  //     response->addHeader("Access-Control-Allow-Origin", "*");
  //     request->send(response);
  //   } });
  // Servir index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!SPIFFS.exists("/index.html")) {
        request->send(404, "text/html", "File not found");
    } else {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html", "text/html");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    } });

  // Página principal
  server.on("/", HTTP_GET, handleHome);

  // Servir archivos estáticos desde la carpeta /assets/
  // server.serveStatic("/assets/", SPIFFS, "/assets/").setDefaultFile("index.html").setCacheControl("max-age=600");
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Rutas API
  server.on("/api/login", HTTP_POST, handleLogin);
  server.on("/api/factory-reset", HTTP_POST, handleFactoryReset);
  server.on("/api/set/wifi", HTTP_POST, handleSetWiFi);
  server.on("/api/set/cells", HTTP_POST, handleSetCells);
  server.on("/api/get/cells", HTTP_GET, handleGetCells);
  server.on("/api/set/tower-info", HTTP_POST, handleSetTowerInfo);
  server.on("/api/get/tower-info", HTTP_GET, handleGetTowerInfo);
  server.on("/api/led", HTTP_POST, handleToggleLed);
  // CORS preflight requests
  server.on("/api/set/tower-info", HTTP_OPTIONS, [](AsyncWebServerRequest *request)
            {
    AsyncWebServerResponse *response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    request->send(response); });
}
