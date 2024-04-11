#include "Joystick.h"
#include "Constants.h"

Joystick::Joystick() {}

void Joystick::begin() {
  this->SerialBT.begin("Telepropulsion_Magnetique"); // Nom du périphérique Bluetooth
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

void Joystick::updateJoystick() {
  // put your main code here, to run repeatedly:
  this->x = analogRead(X);
  this->y = analogRead(Y);

  // sensorValueX = map(sensorValueX, 396,1023,-100,100);
  // sensorValueY = map(sensorValueY, 410,1023,-100,100);

  // if(this->x < this->minX){
  //   this->minX = this->x;
  // }
  // if(this->x > this->maxX){
  //   this->maxX = this->x;
  // }
  // if(this->y < this->minY){
  //   this->minY = this->y;
  // }
  // if(this->y > this->maxY){
  //   this->maxY = this->y;
  // }

  this->centerX = (this->minX + this->maxX) / 2;
  this->centerY = (this->minY + this->maxY) / 2;

  // Serial.print("The X and Y coordinate is:");
  // Serial.print(this->x, DEC);
  // Serial.print(",");
  // Serial.print(this->y, DEC);
  // Serial.print(" \tThe min and max of X and Y coordinate are:\t");
  // Serial.print(this->minX, DEC);
  // Serial.print(",");
  // Serial.print(this->maxX, DEC);
  // Serial.print(",");
  // Serial.print(this->minY, DEC);
  // Serial.print(",");
  // Serial.print(this->maxY, DEC);
  // Serial.print(" \tThe center of X and Y coordinate are:\t");
  // Serial.print(this->centerX, DEC);
  // Serial.print(",");
  // Serial.print(this->centerY, DEC);

  // if(this->x > this->centerX){
  //   this->x = ((this->x - this->centerX) * 100) / (this->maxX - this->centerX);
  // }
  // else{
  //   this->x = ((this->x - this->centerX) * 100) / (this->centerX - this->minX);
  // }
  // if(this->y > this->centerY){
  //   this->y = -((this->y - this->centerY) * 100) / (this->maxY - this->centerY);
  // }
  // else{
  //   this->y = -((this->y - this->centerY) * 100) / (this->centerY - this->minY);
  // }

  
  this->x = MapAdjustment(this->x, this->centerX, this->minX, this->maxX);
  this->y = MapAdjustment(this->y, this->centerY, this->minY, this->maxY);
  // Serial.print("The X and Y coordinate is:");
  // // Serial.print(sensorValueX - sensorCenterX, DEC);
  // // Serial.print(",");
  // // Serial.println(sensorValueY - sensorCenterY, DEC);
  // Serial.print(this->x, DEC);
  // Serial.print(",");
  // Serial.println(this->y, DEC);
  
  // Serial.println("Hello World!");
  // delay(200);
}

void Joystick::updateJoystickMobile(int x, int y) {
  // put your main code here, to run repeatedly:
  this->x = x;
  this->y = y;
}

int Joystick::MapAdjustment(int value, int center, int min, int max){
  if(value > center){
    value = ((value - center) * 100) / (max - center);
  }
  else{
    value = ((value - center) * 100) / (center - min);
  }
  return value;
}