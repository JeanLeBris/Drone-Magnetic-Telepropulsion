#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include "Constants.h"

class Joystick {
  public:
    Joystick();
    int getX();
    int getY();
    void setX(int value);
    void setY(int value);
    void updateJoystick();
    int MapAdjustment(int value, int center, int min, int max);

    int x;
    int y;
    const int minX = 848;
    const int maxX = 2843;
    const int minY = 825;
    const int maxY = 2893;
    int centerX;
    int centerY;
    
  private:

};

#endif
