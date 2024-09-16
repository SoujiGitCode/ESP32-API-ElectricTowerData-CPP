#include "CellsCalibrationFactors.h"
#include <Preferences.h>

Preferences preferences;

CellsCalibrationFactors::CellsCalibrationFactors()
{
    // Iniciar el espacio de preferencias
    preferences.begin("cells_Namespace", false);

    // Verificar y definir valores de calibración individualmente
    double cell_1 = preferences.getDouble("cell_1", -1);
    if (cell_1 == -1)
    {
        Serial.println("cell_1 no está definido. Inicializando a 0.7...");
        preferences.putDouble("cell_1", 0.7);
    }

    double cell_2 = preferences.getDouble("cell_2", -1);
    if (cell_2 == -1)
    {
        Serial.println("cell_2 no está definido. Inicializando a 0.7...");
        preferences.putDouble("cell_2", 0.7);
    }

    double cell_3 = preferences.getDouble("cell_3", -1);
    if (cell_3 == -1)
    {
        Serial.println("cell_3 no está definido. Inicializando a 0.7...");
        preferences.putDouble("cell_3", 0.7);
    }

    double cell_4 = preferences.getDouble("cell_4", -1);
    if (cell_4 == -1)
    {
        Serial.println("cell_4 no está definido. Inicializando a 0.7...");
        preferences.putDouble("cell_4", 0.7);
    }

    // Verificar y definir primeras lecturas individualmente
    double first_read_1 = preferences.getDouble("first_read_1", -1);
    if (first_read_1 == -1)
    {
        Serial.println("first_read_1 no está definido. Inicializando a 0.7...");
        preferences.putDouble("first_read_1", 0.7);
    }

    double first_read_2 = preferences.getDouble("first_read_2", -1);
    if (first_read_2 == -1)
    {
        Serial.println("first_read_2 no está definido. Inicializando a 0.7...");
        preferences.putDouble("first_read_2", 0.7);
    }

    double first_read_3 = preferences.getDouble("first_read_3", -1);
    if (first_read_3 == -1)
    {
        Serial.println("first_read_3 no está definido. Inicializando a 0.7...");
        preferences.putDouble("first_read_3", 0.7);
    }

    double first_read_4 = preferences.getDouble("first_read_4", -1);
    if (first_read_4 == -1)
    {
        Serial.println("first_read_4 no está definido. Inicializando a 0.7...");
        preferences.putDouble("first_read_4", 0.7);
    }

    // Cerrar el espacio de preferencias
    preferences.end();
}

void CellsCalibrationFactors::initializePreferences()
{
    preferences.begin("cells_Namespace", false);

    Serial.println("Checking and setting default values for cells...");

    // Verificar cada valor y establecerlo en 0.7 si no está definido
    if (preferences.getDouble("cell_1", -1) == -1)
    {
        preferences.putDouble("cell_1", 0.7);
        Serial.println("cell_1 initialized to 0.7");
    }
    if (preferences.getDouble("cell_2", -1) == -1)
    {
        preferences.putDouble("cell_2", 0.7);
        Serial.println("cell_2 initialized to 0.7");
    }
    if (preferences.getDouble("cell_3", -1) == -1)
    {
        preferences.putDouble("cell_3", 0.7);
        Serial.println("cell_3 initialized to 0.7");
    }
    if (preferences.getDouble("cell_4", -1) == -1)
    {
        preferences.putDouble("cell_4", 0.7);
        Serial.println("cell_4 initialized to 0.7");
    }

    // Inicializar first_read_* con 0.7 si no están definidos
    if (preferences.getDouble("first_read_1", -1) == -1)
    {
        preferences.putDouble("first_read_1", 0.7);
        Serial.println("first_read_1 initialized to 0.7");
    }
    if (preferences.getDouble("first_read_2", -1) == -1)
    {
        preferences.putDouble("first_read_2", 0.7);
        Serial.println("first_read_2 initialized to 0.7");
    }
    if (preferences.getDouble("first_read_3", -1) == -1)
    {
        preferences.putDouble("first_read_3", 0.7);
        Serial.println("first_read_3 initialized to 0.7");
    }
    if (preferences.getDouble("first_read_4", -1) == -1)
    {
        preferences.putDouble("first_read_4", 0.7);
        Serial.println("first_read_4 initialized to 0.7");
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