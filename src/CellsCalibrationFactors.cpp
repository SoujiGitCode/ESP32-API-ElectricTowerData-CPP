#include "CellsCalibrationFactors.h"
#include <Preferences.h>

Preferences preferences;

CellsCalibrationFactors::CellsCalibrationFactors()
{
    initializePreferences();
}

void CellsCalibrationFactors::initializePreferences()
{
    preferences.begin("cells_Namespace", false);

    if (preferences.getBool("defaultValues", true))
    {
        Serial.println("Setting default values for cells...");

        preferences.putDouble("first_cell", 99.0);
        preferences.putDouble("second_cell", 99.0);
        preferences.putDouble("third_cell", 99.0);
        preferences.putDouble("fourth_cell", 99.0);

        // Inicializar first_read_* con valores predeterminados
        preferences.putDouble("first_read_1", 2.0);
        preferences.putDouble("first_read_2", 3.0);
        preferences.putDouble("first_read_3", 4.0);
        preferences.putDouble("first_read_4", 5.0);

        // Confirmar que los valores se han guardado correctamente
        Serial.println("first_read_1 set to: " + String(preferences.getDouble("first_read_1", -1)));
        Serial.println("first_read_2 set to: " + String(preferences.getDouble("first_read_2", -1)));
        Serial.println("first_read_3 set to: " + String(preferences.getDouble("first_read_3", -1)));
        Serial.println("first_read_4 set to: " + String(preferences.getDouble("first_read_4", -1)));

        noMoreFactoryValues();
    }
    preferences.end();
}

void CellsCalibrationFactors::noMoreFactoryValues()
{
    preferences.begin("cells_Namespace", false);
    preferences.putBool("defaultValues", false); // Desactiva la escritura de valores predeterminados
    preferences.end();
}

void CellsCalibrationFactors::set_first_cell(double first_cell)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("first_cell", first_cell);
    preferences.end();
}

double CellsCalibrationFactors::get_first_cell()
{
    preferences.begin("cells_Namespace", false);
    double first_cell = preferences.getDouble("first_cell", 0.0);
    preferences.end();
    return first_cell;
}

void CellsCalibrationFactors::set_second_cell(double second_cell)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("second_cell", second_cell);
    preferences.end();
}

double CellsCalibrationFactors::get_second_cell()
{
    preferences.begin("cells_Namespace", false);
    double second_cell = preferences.getDouble("second_cell", 0.0);
    preferences.end();
    return second_cell;
}

void CellsCalibrationFactors::set_third_cell(double third_cell)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("third_cell", third_cell);
    preferences.end();
}

double CellsCalibrationFactors::get_third_cell()
{
    preferences.begin("cells_Namespace", false);
    double third_cell = preferences.getDouble("third_cell", 0.0);
    preferences.end();
    return third_cell;
}

void CellsCalibrationFactors::set_fourth_cell(double fourth_cell)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("fourth_cell", fourth_cell);
    preferences.end();
}

double CellsCalibrationFactors::get_fourth_cell()
{
    preferences.begin("cells_Namespace", false);
    double fourth_cell = preferences.getDouble("fourth_cell", 0.0);
    preferences.end();
    return fourth_cell;
}

// Nuevos métodos para first_read_*

void CellsCalibrationFactors::set_first_read_1(double value)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("first_read_1", value);
    preferences.end();
}

double CellsCalibrationFactors::get_first_read_1()
{
    preferences.begin("cells_Namespace", false);
    double value = preferences.getDouble("first_read_1", 0.0);
    preferences.end();
    return value;
}

void CellsCalibrationFactors::set_first_read_2(double value)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("first_read_2", value);
    preferences.end();
}

double CellsCalibrationFactors::get_first_read_2()
{
    preferences.begin("cells_Namespace", false);
    double value = preferences.getDouble("first_read_2", 0.0);
    preferences.end();
    return value;
}

void CellsCalibrationFactors::set_first_read_3(double value)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("first_read_3", value);
    preferences.end();
}

double CellsCalibrationFactors::get_first_read_3()
{
    preferences.begin("cells_Namespace", false);
    double value = preferences.getDouble("first_read_3", 0.0);
    preferences.end();
    return value;
}

void CellsCalibrationFactors::set_first_read_4(double value)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("first_read_4", value);
    preferences.end();
}

double CellsCalibrationFactors::get_first_read_4()
{
    preferences.begin("cells_Namespace", false);
    double value = preferences.getDouble("first_read_4", 0.0);
    preferences.end();
    return value;
}