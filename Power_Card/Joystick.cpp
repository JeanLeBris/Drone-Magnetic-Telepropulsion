#include "Joystick.h"
#include "Constants.h"

Joystick::Joystick() {}

void Joystick::begin() {
  this->SerialBT.begin("Teleprop_Magnetique"); // Nom du périphérique Bluetooth
}

int Joystick::getX() {
  return this->x;
}

int Joystick::getY() {
  return this->y;
}

void Joystick::setX(int value) {
  this->x = value;
}

void Joystick::setY(int value) {
  this->y = value;
}

void Joystick::updateJoystickMobile(int x, int y) {
  this->x = x;
  this->y = y;
}
