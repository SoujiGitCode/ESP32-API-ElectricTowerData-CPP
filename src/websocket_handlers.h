#pragma once
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "LoadCells.h"
#include <iostream>
#include <iomanip>

void configureRoutes(AsyncWebServer &server);
void webSocketEvent(WebSocketsServer &webSocket, uint8_t num, WStype_t type, uint8_t *payload, size_t length);
// void sendDataOverWebSocket(DynamicJsonDocument &doc, WebSocketsServer &webSocket);
void sendDataOverWebSocket(DynamicJsonDocument &doc, WebSocketsServer &webSocket, LoadCells &loadCells);
double gramosANewtons(double gramos);
