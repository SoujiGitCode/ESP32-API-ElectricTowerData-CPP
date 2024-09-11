#pragma once
#include <ESPAsyncWebServer.h>

void configureRoutes(AsyncWebServer &server);
void handleLogin(AsyncWebServerRequest *request);
void handleWelcome(AsyncWebServerRequest *request);
void handleFactoryReset(AsyncWebServerRequest *request);
void handleSetWiFi(AsyncWebServerRequest *request);
void handleSetCells(AsyncWebServerRequest *request);
void handleGetTowerInfo(AsyncWebServerRequest *request);
void handleSetTowerInfo(AsyncWebServerRequest *request);
void handleToggleLed(AsyncWebServerRequest *request);
