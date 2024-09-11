#ifndef CellsCalibrationFactors_H
#define CellsCalibrationFactors_H

#include <Arduino.h>
#include <Preferences.h> // Asegúrate de incluir esta biblioteca

class CellsCalibrationFactors
{
public:
    CellsCalibrationFactors();

    void set_first_cell(double first_cell);
    double get_first_cell();

    void set_second_cell(double second_cell);
    double get_second_cell();

    void set_third_cell(double third_cell);
    double get_third_cell();

    void set_fourth_cell(double fourth_cell);
    double get_fourth_cell();

    void set_first_read_1(double value);
    double get_first_read_1();

    void set_first_read_2(double value);
    double get_first_read_2();

    void set_first_read_3(double value);
    double get_first_read_3();

    void set_first_read_4(double value);
    double get_first_read_4();

private:
    Preferences preferences; // Instancia de la clase Preferences
    String first_cellStr, second_cellStr, third_cellStr, fourth_cellStr;

    void initializePreferences(); // Inicializa los valores predeterminados
    void noMoreFactoryValues();   // Marca cuando se han aplicado los valores por defecto
};

#endif
