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

        preferences.putDouble("cell_1", 99.0);
        preferences.putDouble("cell_2", 99.0);
        preferences.putDouble("cell_3", 99.0);
        preferences.putDouble("cell_4", 99.0);

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

void CellsCalibrationFactors::set_first_cell(double cell_1)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("cell_1", cell_1);
    preferences.end();
}

double CellsCalibrationFactors::get_first_cell()
{
    preferences.begin("cells_Namespace", false);
    double cell_1 = preferences.getDouble("cell_1", 0.0);
    preferences.end();
    return cell_1;
}

void CellsCalibrationFactors::set_second_cell(double cell_2)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("cell_2", cell_2);
    preferences.end();
}

double CellsCalibrationFactors::get_second_cell()
{
    preferences.begin("cells_Namespace", false);
    double cell_2 = preferences.getDouble("cell_2", 0.0);
    preferences.end();
    return cell_2;
}

void CellsCalibrationFactors::set_third_cell(double cell_3)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("cell_3", cell_3);
    preferences.end();
}

double CellsCalibrationFactors::get_third_cell()
{
    preferences.begin("cells_Namespace", false);
    double cell_3 = preferences.getDouble("cell_3", 0.0);
    preferences.end();
    return cell_3;
}

void CellsCalibrationFactors::set_fourth_cell(double cell_4)
{
    preferences.begin("cells_Namespace", false);
    preferences.putDouble("cell_4", cell_4);
    preferences.end();
}

double CellsCalibrationFactors::get_fourth_cell()
{
    preferences.begin("cells_Namespace", false);
    double cell_4 = preferences.getDouble("cell_4", 0.0);
    preferences.end();
    return cell_4;
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