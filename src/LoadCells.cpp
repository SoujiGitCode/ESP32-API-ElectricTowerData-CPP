#include "LoadCells.h"
#include "CellsCalibrationFactors.h"

extern CellsCalibrationFactors Cells;

LoadCells::LoadCells()
{
    Serial.println("Loadcells constructor being called");
}

void LoadCells::initialize(int cell_number, double storedCalibration, int DTPin, int SCKPin)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 5000 / portTICK_PERIOD_MS; // 5000 ms de estabilización
    xLastWakeTime = xTaskGetTickCount();

    Serial.print("stored Calibration factor for hound :");
    Serial.println(cell_number);
    Serial.println(storedCalibration);

    _loadCells[cell_number - 1] = new HX711_ADC(DTPin, SCKPin);
    _loadCells[cell_number - 1]->begin();

    _loadCells[cell_number - 1]->setCalFactor(storedCalibration);
    // _loadCells[cell_number - 1]->setCalFactor(-17.07);

    // Iniciar la estabilización
    bool _tare = false; // No tarear
    _loadCells[cell_number - 1]->start(xFrequency, _tare);
    if (_loadCells[cell_number - 1]->getTareTimeoutFlag())
    {
        Serial.println("Timeout, check MCU > HX711 wiring and pin designations");
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency); // Esperar la estabilización

    _loadCells[cell_number - 1]->update();

    if (!_loadCells[cell_number - 1]->update())
    {
        Serial.print("loadcell is not ready. Number:");
        Serial.println(cell_number);
        loadCellReadings[cell_number - 1] = _loadCells[cell_number - 1]->getData();
    }
    else
    {
        Serial.print("loadcell is ready. Number:");
        Serial.println(cell_number);
        loadCellReadings[cell_number - 1] = _loadCells[cell_number - 1]->getData();
    }
}

double LoadCells::getLoadCellValue(uint8_t index, double calibrated1stRead)
{
    if (index >= 1 && index <= 4)
    {
        // Obtiene el valor crudo
        double rawData = _loadCells[index - 1]->getData();

        // Asegurarse de que rawData sea positivo
        rawData = fabs(rawData);

        // Resta siempre el mayor valor al menor
        double correctedData;
        if (rawData > calibrated1stRead)
        {
            correctedData = rawData - calibrated1stRead;
        }
        else
        {
            correctedData = calibrated1stRead - rawData;
        }

        return correctedData;
    }
    else
    {
        return NAN;
    }
}
