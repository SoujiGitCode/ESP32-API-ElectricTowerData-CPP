#include "functions.h"
#include <ArduinoJson.h>

String serializeTowerInfo(const TowerInfo &info)
{
    ArduinoJson::DynamicJsonDocument doc(1024);

    doc["id"] = info.id;
    doc["name"] = info.name;
    doc["slang"] = info.slang;
    doc["location"] = info.location;
    doc["priority"] = info.priority;
    doc["type"] = info.type;
    doc["loadcells_amount"] = info.loadcells_amount;

    String json;
    serializeJson(doc, json);
    return json;
}

TowerInfo deserializeTowerInfo(const String &json)
{
    ArduinoJson::DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);

    TowerInfo info;
    info.id = doc["id"].as<String>();
    info.name = doc["name"].as<String>();
    info.slang = doc["slang"].as<String>();
    info.location = doc["location"].as<String>();
    info.priority = doc["priority"].as<int>(); // Asumiendo que priority es un entero
    info.type = doc["type"].as<String>();
    info.loadcells_amount = doc["loadcells_amount"].as<int>(); // Asumiendo que loadcells_amount es un entero

    return info;
}

// Implementación de otras funciones comunes
