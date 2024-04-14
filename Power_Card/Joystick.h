#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include "Constants.h"

class Joystick {
  public:
    Joystick();
    void begin();
    int getX();
    int getY();
    void setX(int value);
    void setY(int value);
    void updateJoystickMobile(int x, int y);
    BluetoothSerial SerialBT;
    int x;
    int y;
    
  private:

};

#endif
