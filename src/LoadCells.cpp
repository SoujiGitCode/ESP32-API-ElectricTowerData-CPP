#include "LoadCells.h"
#include "CellsCalibrationFactors.h"
#include "esp_task_wdt.h"

extern CellsCalibrationFactors Cells;

LoadCells::LoadCells() : _loadCell(nullptr)
{
    Serial.println("Loadcell constructor being called");
}

void LoadCells::initialize(int cell_number, double storedCalibration, int DTPin, int SCKPin)
{
    Serial.print("Initializing load cell number: ");
    Serial.println(cell_number);

    _loadCell = new HX711(); // Crea el objeto HX711 dinámicamente
    if (_loadCell == nullptr)
    {
        Serial.println("Error: No se pudo crear el objeto HX711.");
        return;
    }

    _loadCell->begin(DTPin, SCKPin);
    Serial.println("Pines inicializados.");

    _calibrationFactor = storedCalibration;
    _loadCell->set_scale(_calibrationFactor);
    Serial.println("Factor de calibración configurado.");

    // Añadir un retraso adicional para permitir que la celda se estabilice
    delay(1000);

    if (_loadCell->wait_ready_timeout(5000))
    {
        loadCellReadings[cell_number - 1] = _loadCell->get_units(10); // Obtener la lectura
        Serial.print("Lectura inicial de la celda ");
        Serial.println(cell_number);
    }
    else
    {
        Serial.print("La celda ");
        Serial.print(cell_number);
        Serial.println(" no está lista después del tiempo de espera.");
        _loadCell = nullptr; // Marca esta celda como no válida
    }
}

double LoadCells::getLoadCellValue()
{
    if (_loadCell != nullptr && _loadCell->is_ready())
    {
        double rawData = _loadCell->get_units(10); // Obtener promedio de 10 lecturas
        return rawData;
    }
    else
    {
        Serial.println("Celda de carga no está lista o no inicializada.");
        return NAN;
    }
}
