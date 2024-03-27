#include "Joystick.h"
#include "Constants.h"

Joystick::Joystick() {}

void Joystick::getX() {
  return this.x;
}

void Joystick::getY() {
  return this.y;
}

void Joystick::setX(int value) {
  this.x = value
}

void Joystick::setY(int value) {
  this.y = value
}
