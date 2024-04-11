#ifndef COIL_H
#define COIL_H

#include <Arduino.h>
#include "Constants.h"

class Coil {
  public:
    Coil();
    void begin(int i);
    void setCoilCurrent(int coilNumber, float current);
    void updateCoil();

    int PWM_pin;
    int dir_pin;
    int coil_number;
    const float resistance = 2.84;

    int power;
    int target_power;
    int direction;
    
  private:
    // const int pwmPins[8] = {2, 4, 5, 12, 13, 14, 15, 16}; // Broches PWM pour les bobines
    // const int pwmPins[8] = {PWM_1, PWM_2, PWM_3, PWM_4, PWM_5, PWM_6, PWM_7, PWM_8}; // Broches PWM pour les bobines
    // const float resistances[8] = {2.84, 2.84, 2.84, 2.84, 2.84, 2.84, 2.84, 2.84}; // Résistances des bobines en ohms
};

#endif
