// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>

struct TowerInfo
{
    String id;
    String name;
    String slang;
    String location;
    int priority;
    String type;
    int loadcells_amount;
};

// Declaraciones de funciones...
String serializeTowerInfo(const TowerInfo &info);
TowerInfo deserializeTowerInfo(const String &json);

#endif // FUNCTIONS_H
