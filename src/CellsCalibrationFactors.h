#ifndef CellsCalibrationFactors_H
#define CellsCalibrationFactors_H

#include <Arduino.h>
#include <Preferences.h> // Asegúrate de incluir esta biblioteca

class CellsCalibrationFactors
{
public:
    CellsCalibrationFactors();

    void set_first_cell(float first_cell);
    float get_first_cell();

    void set_second_cell(float second_cell);
    float get_second_cell();

    void set_third_cell(float third_cell);
    float get_third_cell();

    void set_fourth_cell(float fourth_cell);
    float get_fourth_cell();

    void set_first_read_1(float value);
    float get_first_read_1();

    void set_first_read_2(float value);
    float get_first_read_2();

    void set_first_read_3(float value);
    float get_first_read_3();

    void set_first_read_4(float value);
    float get_first_read_4();

private:
    Preferences preferences; // Instancia de la clase Preferences
    String first_cellStr, second_cellStr, third_cellStr, fourth_cellStr;

    void initializePreferences(); // Inicializa los valores predeterminados
    void noMoreFactoryValues();   // Marca cuando se han aplicado los valores por defecto
};

#endif
