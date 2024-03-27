#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include "Constants.h"

class Joystick {
  public:
    Joystick();
    void getX();
    void getY();
    void setX(int value);
    void setY(int value);
    
  private:
    int x;
    int y;
    // const int pwmPins[8] = {2, 4, 5, 12, 13, 14, 15, 16}; // Broches PWM pour les bobines
    // const int pwmPins[8] = {PWM_1, PWM_2, PWM_3, PWM_4, PWM_5, PWM_6, PWM_7, PWM_8}; // Broches PWM pour les bobines
    // const float resistances[8] = {2.84, 2.84, 2.84, 2.84, 2.84, 2.84, 2.84, 2.84}; // Résistances des bobines en ohms
};

#endif
