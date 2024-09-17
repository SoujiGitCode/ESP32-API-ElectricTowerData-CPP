#include "LoadCells.h"
#include "CellsCalibrationFactors.h"
#include "esp_task_wdt.h"

extern CellsCalibrationFactors Cells;

LoadCells::LoadCells() : _loadCell(nullptr)
{
    Serial.println("Loadcell constructor being called");
}

void LoadCells::initialize(int cell_number, float storedCalibration, float storedRead, int DTPin, int SCKPin)
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

    // Verificar si el factor de calibración es válido antes de asignarlo
    if (storedCalibration <= 0.0)
    {
        Serial.println("Advertencia: El factor de calibración almacenado es inválido o no definido.");
        _calibrationFactor = 1.0; // Asigna un valor predeterminado de 1.0
    }
    else
    {

        _calibrationFactor = storedCalibration;
    }

    _loadCell->set_scale(_calibrationFactor);
    Serial.print("Factor de calibración configurado para celda numero: ");
    Serial.println(cell_number);
    float getCalibrationFactor = _loadCell->get_scale();
    Serial.print("getScale:de celda ");
    Serial.print(getCalibrationFactor);

    _loadCell->set_offset(storedRead);
    Serial.print("Seteado de offset  : ");
    Serial.print(storedRead);
    // Añadir un retraso adicional para permitir que la celda se estabilice
    vTaskDelay(1000 / portTICK_PERIOD_MS); // lee los valores cada 1000 ms

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
        if (rawData == INFINITY)
        {
            Serial.println("Error: Lectura retornó infinito.");
            return NAN;
        }
        return fabs(rawData);
    }
    else
    {
        Serial.println("Celda de carga no está lista o no inicializada.");
        return NAN;
    }
}
