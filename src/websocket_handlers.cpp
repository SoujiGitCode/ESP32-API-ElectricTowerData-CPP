#include "websocket_handlers.h"

extern double loadCellReadings[4];

double gramosANewtons(double gramos)
{
  const double g = 9.81;             // Aceleración debida a la gravedad en m/s^2
  double masaEnKg = gramos / 1000.0; // Convertir gramos a kilogramos
  double fuerza = masaEnKg * g;      // Calcular la fuerza (tensión) en newtons
  return fuerza;
}

void webSocketEvent(WebSocketsServer &webSocket, uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
  case WStype_CONNECTED:
  {
    Serial.printf("[%u] Connected from url: %s\n", num, payload);
    // send message to client
    webSocket.sendTXT(num, "Connected");
  }
  break;
  case WStype_TEXT:
    Serial.printf("[%u] Message: %s\n", num, payload);
    break;
  case WStype_BIN:
    Serial.printf("[%u] Message [BINARY]: %u bytes\n", num, length);
    break;
  }
}

void sendDataOverWebSocket(DynamicJsonDocument &doc, WebSocketsServer &webSocket, LoadCells &loadCells)
{
  static unsigned long last_time = 0;
  if (millis() - last_time >= 500)
  { // envía los datos cada 1 segundo
    last_time = millis();

    JsonObject data = doc.to<JsonObject>();
    JsonObject sections = data.createNestedObject("sections");
    JsonObject a = sections.createNestedObject("a");

    // Usa los valores de loadCellReadings para enviar los datos
    a["a1"] = gramosANewtons(loadCellReadings[0]);
    a["a2"] = gramosANewtons(loadCellReadings[1]);
    a["a3"] = gramosANewtons(loadCellReadings[2]);
    a["a4"] = gramosANewtons(loadCellReadings[3]);

    // a["a1"] = loadCellReadings[0];
    // a["a2"] = loadCellReadings[1];
    // a["a3"] = loadCellReadings[2];
    // a["a4"] = loadCellReadings[3];

    // Serializa y envía el objeto JSON
    String output;
    serializeJson(doc, output);
    webSocket.broadcastTXT(output);
  }
}
