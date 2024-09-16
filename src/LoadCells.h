#ifndef LoadCells_h
#define LoadCells_h

#include "Arduino.h"
#include "HX711.h"

extern double loadCellReadings[4];

class LoadCells
{
public:
    LoadCells();
    void initialize(int cell_number, double storedCalibration, int DTPin, int SCKPin);
    double getLoadCellValue();

private:
    HX711 *_loadCell; // Ahora solo tienes un puntero a HX711 por instancia
    double _calibrationFactor;
};

#endif
