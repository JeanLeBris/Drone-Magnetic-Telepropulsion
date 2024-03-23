#ifndef COILCONTROL_H
#define COILCONTROL_H

#include <Arduino.h>

class CoilControl {
  public:
    CoilControl();
    void begin();
    void setCoilCurrent(int coilNumber, float current);
    
  private:
    const int pwmPins[8] = {2, 4, 5, 12, 13, 14, 15, 16}; // Broches PWM pour les bobines
    const float resistances[8] = {100, 100, 100, 100, 100, 100, 100, 100}; // Résistances des bobines en ohms
};

#endif
