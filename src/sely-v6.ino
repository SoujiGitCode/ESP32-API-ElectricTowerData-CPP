#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "config.h"
#include "CellsCalibrationFactors.h"
#include "route_handlers.h"
#include "websocket_handlers.h"
#include <SPIFFS.h>
#include <Ticker.h>
#include <MD5Builder.h>
#include "LoadCells.h"
#include "nvs_flash.h"
Ticker restartTimer;
// Declaramos el pin que encendera
int ledPin = 2;

//--------------------CHIP ID HASHED ACA-----------------//
String chipIDHashed = "37f1e4d7f7ba4a2a11e64f3750f48085";
//-------------------------------------------------------//
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(8080);
ArduinoJson::DynamicJsonDocument doc(1024);
volatile bool factoryResetTriggered = false;
// La instancia de Config y la función de interrupción deben ser globales
Config config;
CellsCalibrationFactors Cells;
double calibrationFactor;
LoadCells loadCells;
LoadCells first_cell;
LoadCells second_cell;
LoadCells third_cell;
LoadCells fourth_cell;
// Definir esta variable globalmente para usarla en todo el archivo
double loadCellReadings[4] = {0.0, 0.0, 0.0, 0.0};
// Definir globalmente el primer valor de lectura hecho en la celda de carga una vez calibrada (hacerla con las 4) y con 0g encima
double loadCellfirst_read_AfterCalibration[4] = {605242, 478409, 491474, 489537};

//--------------------Cantidad de Celdas de Carga Aca-----------------//
// Class para Cells
struct HoundParams
{
  int cell_number;
  double storedCalibration;
  int DTPin;
  int SCKPin;
};

void antiCloneFunction(String chipIDHashed)
{
  uint64_t currentChipId = ESP.getEfuseMac();          // Obtener el Chip ID del ESP32 actual
  char currentChipIdString[21];                        // Se creará una cadena de caracteres para almacenar el chip ID
  sprintf(currentChipIdString, "%llu", currentChipId); // Convierte el chip ID a una cadena de caracteres

  MD5Builder md5;
  md5.begin();
  md5.add(currentChipIdString);
  md5.calculate();
  String currentChipIdHashed = md5.toString();

  if (chipIDHashed != currentChipIdHashed)
  {
    ESP.restart(); // Si no coinciden, reiniciar ESP32
  }
  // Si coinciden, no hacer nada y continuar con el ciclo de vida
}

void performReset()
{
  config.resetToFactoryDefaults();
  ESP.restart();
}

void resetToFactoryDefaultsInterrupt()
{
  restartTimer.once(0.1, performReset); // 0.1 segundos de retardo antes de llamar a performReset
}

void initializeLoadCellsCheck(LoadCells &hound, HoundParams &params)
{
  hound.initialize(params.cell_number, params.storedCalibration, params.DTPin, params.SCKPin);
}

// Estas tareas serán ejecutadas en el primer y segundo núcleo respectivamente
TaskHandle_t Task1;

void Task1code(void *pvParameters)
{
  for (;;)
  {
    // updateLoadCellReadings()
    webSocket.loop();
    sendDataOverWebSocket(doc, webSocket, loadCells);
    vTaskDelay(100 / portTICK_PERIOD_MS); // lee los valores cada 100 ms
  }
}

// Nueva función que lee los valores de las celdas de carga
void updateLoadCellReadings()
{
  // vTaskDelay(100 / portTICK_PERIOD_MS); // lee los valores cada 100 ms
  loadCellReadings[0] = first_cell.getLoadCellValue(1, loadCellfirst_read_AfterCalibration[0]);
  Serial.print("Loadcell 1: ");
  Serial.println(loadCellReadings[0]);

  loadCellReadings[1] = second_cell.getLoadCellValue(2, loadCellfirst_read_AfterCalibration[1]);
  Serial.print("Loadcell 2: ");
  Serial.println(loadCellReadings[1]);

  loadCellReadings[2] = third_cell.getLoadCellValue(3, loadCellfirst_read_AfterCalibration[2]);
  Serial.print("Loadcell 3: ");
  Serial.println(loadCellReadings[2]);

  loadCellReadings[3] = fourth_cell.getLoadCellValue(4, loadCellfirst_read_AfterCalibration[3]);
  Serial.print("Loadcell 4: ");
  Serial.println(loadCellReadings[3]);
}

// Nueva tarea para leer regularmente los valores de las celdas de carga
void loadCellReadingTask(void *pvParameters)
{
  for (;;)
  {
    // Llama al método `update()` para cada celda de carga aquí
    first_cell._loadCells[0]->update();
    second_cell._loadCells[0]->update();
    third_cell._loadCells[0]->update();
    fourth_cell._loadCells[0]->update();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    updateLoadCellReadings();
  }
}

void setup()
{
  // Inicializar NVS (esto es esencial para que Preferences funcione correctamente)
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    // Si hay un error, borramos la partición NVS y la volvemos a inicializar
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  // Instanciar CellsCalibrationFactors (esto debería ejecutar el constructor)
  CellsCalibrationFactors Cells;
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  pinMode(ledPin, OUTPUT);   // Configuracion inicial del PIN
  digitalWrite(ledPin, LOW); // asegurar que el LED esté apagado al inicio

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  // Se cargan los valores de las primeras lecturas en las celdas de carga almacenados
  loadCellfirst_read_AfterCalibration[0] = Cells.get_first_read_1();
  loadCellfirst_read_AfterCalibration[1] = Cells.get_first_read_2();
  loadCellfirst_read_AfterCalibration[2] = Cells.get_first_read_3();
  loadCellfirst_read_AfterCalibration[3] = Cells.get_first_read_4();

  //--------------------Numero de Celda, Calibration Factor, DT Pin, SKC Pin-----------------//
  HoundParams first_cellParams = {1, Cells.get_first_cell(), 17, 5}; // Reemplaza estos valores con los parámetros reales de cada 'hound'
  HoundParams second_cellParams = {2, Cells.get_second_cell(), 16, 4};
  HoundParams third_cellParams = {3, Cells.get_third_cell(), 26, 27};
  HoundParams fourth_cellParams = {4, Cells.get_fourth_cell(), 14, 12};
  //-------------------------------------------------------//
  // antiCloneFunction(chipIDHashed); // Llamar a la función antiClone

  const char *ssid = config.getSsid();
  const char *password = config.getPassword();

  const int buttonPin = 0; // El botón está conectado al GPIO 0
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), resetToFactoryDefaultsInterrupt, FALLING);

  Serial.println(config.getCurrentConfig());
  Serial.println(ssid);
  Serial.println(password);

  WiFi.softAPdisconnect(true);
  WiFi.softAP(ssid, password);
  configureRoutes(server);
  server.begin();

  // Iniciar el servidor WebSocket
  webSocket.begin();
  Serial.println("Servidor WebSocket iniciado en el puerto 8080.");

  // Configurar el manejador de eventos del WebSocket
  webSocket.onEvent([webSocketPtr = &webSocket](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
                    { webSocketEvent(*webSocketPtr, num, type, payload, length); });
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }

  Serial.println("Webserver Started!");
  calibrationFactor = Cells.get_first_cell();
  Serial.print("Calibration factor before constructor: ");
  Serial.println(calibrationFactor);

  // Llamada a la función initializeLoadCellsCheck
  initializeLoadCellsCheck(first_cell, first_cellParams);
  initializeLoadCellsCheck(second_cell, second_cellParams);
  initializeLoadCellsCheck(third_cell, third_cellParams);
  initializeLoadCellsCheck(fourth_cell, fourth_cellParams);

  // DEfinicion de tareas en nucleo 0 y 1
  // xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 1);

  // xTaskCreatePinnedToCore(loadCellReadingTask, "LoadCellReadingTask", 10000, NULL, 1, NULL, 0);
}
void loop()
{
  // Deja vacío el loop en este caso
}
