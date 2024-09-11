#ifndef LoadCells_h
#define LoadCells_h

#include "Arduino.h"
#include "HX711_ADC.h"

extern double loadCellReadings[4];

class LoadCells
{
public:
    LoadCells();
    void initialize(int cell_number, double storedCalibration, int DTPin, int SCKPin);
    double getLoadCellValue(uint8_t index, double calibrated1stRead);
    HX711_ADC *_loadCells[4];

private:
    double _calibrationFactor;
};

#endif
