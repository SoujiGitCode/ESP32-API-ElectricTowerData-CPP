#include "CellsCalibrationFactors.h"
#include <Preferences.h>

Preferences preferences;

CellsCalibrationFactors::CellsCalibrationFactors()
{
    // Iniciar el espacio de preferencias solo para lectura
    preferences.begin("cells_Namespace", true);

    Serial.println("Verificando valores almacenados en las preferencias...");

    // Leer y mostrar los valores de calibración y las primeras lecturas almacenadas
    float cell_1 = preferences.getFloat("calib_1", -1);
    Serial.print("getcalib_1!: ");
    Serial.println(cell_1);

    float cell_2 = preferences.getFloat("calib_2", -1);
    Serial.print("getcalib_2!: ");
    Serial.println(cell_2);

    float cell_3 = preferences.getFloat("calib_3", -1);
    Serial.print("getcalib_3!: ");
    Serial.println(cell_3);

    float cell_4 = preferences.getFloat("calib_4", -1);
    Serial.print("getcalib_4!: ");
    Serial.println(cell_4);

    // Leer y mostrar las primeras lecturas
    float first_read_1 = preferences.getFloat("read_1", -1);
    Serial.print("Primera lectura para la celda 1!: ");
    Serial.println(first_read_1);

    float first_read_2 = preferences.getFloat("read_2", -1);
    Serial.print("Primera lectura para la celda 2!: ");
    Serial.println(first_read_2);

    float first_read_3 = preferences.getFloat("read_3", -1);
    Serial.print("Primera lectura para la celda 3!: ");
    Serial.println(first_read_3);

    float first_read_4 = preferences.getFloat("read_4", -1);
    Serial.print("Primera lectura para la celda 4!: ");
    Serial.println(first_read_4);

    preferences.end(); // Terminar el acceso a las preferencias
}

void CellsCalibrationFactors::initializePreferences()
{
    preferences.begin("cells_Namespace", false);

    // Serial.println("Checking and setting default values for cells...");

    // // Verificar cada valor y establecerlo en 0.7 si no está definido
    // if (preferences.getFloat("calib_1", -1) == -1)
    // {
    //     preferences.putFloat("calib_1", 0.7);
    //     Serial.println("cell_1 initialized to 0.7");
    // }
    // if (preferences.getFloat("calib_2", -1) == -1)
    // {
    //     preferences.putFloat("calib_2", 0.7);
    //     Serial.println("cell_2 initialized to 0.7");
    // }
    // if (preferences.getFloat("calib_3", -1) == -1)
    // {
    //     preferences.putFloat("calib_3", 0.7);
    //     Serial.println("cell_3 initialized to 0.7");
    // }
    // if (preferences.getFloat("calib_4", -1) == -1)
    // {
    //     preferences.putFloat("calib_4", 0.7);
    //     Serial.println("cell_4 initialized to 0.7");
    // }

    // // Inicializar first_read_* con 0.7 si no están definidos
    // if (preferences.getFloat("read_1", -1) == -1)
    // {
    //     preferences.putFloat("read_1", 0.7);
    //     Serial.println("first_read_1 initialized to 0.7");
    // }
    // if (preferences.getFloat("read_2", -1) == -1)
    // {
    //     preferences.putFloat("read_2", 0.7);
    //     Serial.println("first_read_2 initialized to 0.7");
    // }
    // if (preferences.getFloat("read_3", -1) == -1)
    // {
    //     preferences.putFloat("read_3", 0.7);
    //     Serial.println("first_read_3 initialized to 0.7");
    // }
    // if (preferences.getFloat("read_4", -1) == -1)
    // {
    //     preferences.putFloat("read_4", 0.7);
    //     Serial.println("first_read_4 initialized to 0.7");
    // }

    preferences.end();
}

void CellsCalibrationFactors::noMoreFactoryValues()
{
    preferences.begin("cells_Namespace", false);
    preferences.putBool("defaultValues", false); // Desactiva la escritura de valores predeterminados
    preferences.end();
}

void CellsCalibrationFactors::set_first_cell(float cell_1)
{
    preferences.begin("cells_Namespace", false); // Abrir en modo de escritura
    preferences.putFloat("calib_1", cell_1);
    preferences.end();
}

float CellsCalibrationFactors::get_first_cell()
{
    preferences.begin("cells_Namespace", false);
    float cell_1 = preferences.getFloat("calib_1", 0.0);
    preferences.end();
    Serial.print("getcalib_1!: ");
    Serial.println(cell_1);
    return cell_1;
}

void CellsCalibrationFactors::set_second_cell(float cell_2)
{
    preferences.begin("cells_Namespace", false);
    preferences.putFloat("calib_2", cell_2);
    preferences.end();
}

float CellsCalibrationFactors::get_second_cell()
{
    preferences.begin("cells_Namespace", false);
    float cell_2 = preferences.getFloat("calib_2", 0.0);
    preferences.end();
    Serial.print("getcalib_2!: ");
    Serial.println(cell_2);
    return cell_2;
}

void CellsCalibrationFactors::set_third_cell(float cell_3)
{
    preferences.begin("cells_Namespace", false);
    preferences.putFloat("calib_3", cell_3);
    preferences.end();
}

float CellsCalibrationFactors::get_third_cell()
{
    preferences.begin("cells_Namespace", false);
    float cell_3 = preferences.getFloat("calib_3", 0.0);
    preferences.end();
    return cell_3;
}

void CellsCalibrationFactors::set_fourth_cell(float cell_4)
{
    preferences.begin("cells_Namespace", false);
    preferences.putFloat("calib_4", cell_4);
    preferences.end();
}

float CellsCalibrationFactors::get_fourth_cell()
{
    preferences.begin("cells_Namespace", false);
    float cell_4 = preferences.getFloat("calib_4", 0.0);
    preferences.end();
    return cell_4;
}

// Nuevos métodos para first_read_*

void CellsCalibrationFactors::set_first_read_1(float value)
{
    preferences.begin("cells_Namespace", false);
    preferences.putFloat("read_1", value);
    preferences.end();
}

float CellsCalibrationFactors::get_first_read_1()
{
    preferences.begin("cells_Namespace", false);
    float value = preferences.getFloat("read_1", 0.0);
    preferences.end();
    return value;
}

void CellsCalibrationFactors::set_first_read_2(float value)
{
    preferences.begin("cells_Namespace", false);
    preferences.putFloat("read_2", value);
    preferences.end();
}

float CellsCalibrationFactors::get_first_read_2()
{
    preferences.begin("cells_Namespace", false);
    float value = preferences.getFloat("read_2", 0.0);
    preferences.end();
    return value;
}

void CellsCalibrationFactors::set_first_read_3(float value)
{
    preferences.begin("cells_Namespace", false);
    preferences.putFloat("read_3", value);
    preferences.end();
}

float CellsCalibrationFactors::get_first_read_3()
{
    preferences.begin("cells_Namespace", false);
    float value = preferences.getFloat("read_3", 0.0);
    preferences.end();
    return value;
}

void CellsCalibrationFactors::set_first_read_4(float value)
{
    preferences.begin("cells_Namespace", false);
    preferences.putFloat("read_4", value);
    preferences.end();
}

float CellsCalibrationFactors::get_first_read_4()
{
    preferences.begin("cells_Namespace", false);
    float value = preferences.getFloat("read_4", 0.0);
    preferences.end();
    return value;
}