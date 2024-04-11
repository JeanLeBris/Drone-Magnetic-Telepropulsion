#include "Coil.h"
#include "Constants.h"
#include <Arduino.h>
#include <driver/ledc.h>

Coil::Coil() {}

void Coil::begin(int i) {
 
  /*Info...
	PWM_freq * 2^PWM_resolution cannot exceed the clock speed.
	PWM frequency and resolution are interdependent. The higher the PWM frequency, the lower the duty cycle resolution (and vice versa).
	clock source is at 80 MHz
	
	Pour F_PWM = 10KHz Resolution Max = 12 bits
  */
  
  // Initialisation des broches en mode de sortie
  this->PWM_pin = PWM_pins[i];
  this->dir_pin = dir_pins[i];
  pinMode(this->PWM_pin, OUTPUT);
  pinMode(this->dir_pin, OUTPUT);
  ledcSetup(i, 10000, 12); // Fréquence de 10 kHz, résolution de 12 bits
  ledcAttachPin(this->PWM_pin, i);
  this->coil_number = i;
  this->power = 0;
  // ledcWrite(this->coil_number, 500);
  // digitalWrite(this->dir_pin, LOW);
  // delay(100);
  // digitalWrite(this->dir_pin, HIGH);
  // delay(100);
  // ledcWrite(this->coil_number, 0);
  this->direction = 1;
  digitalWrite(this->dir_pin, LOW);
  this->power = 0;
  this->target_power = 0;
}

void Coil::setCoilCurrent(int coilNumber, float current) {
  if(coilNumber >= 0 && coilNumber < 8) {
    float voltage = current * resistance;
    int dutyCycle = (voltage / 24) * 4095; // L'alimentation est de 24V

    if(dutyCycle > 4095) dutyCycle = 4095; // Limiter à la valeur maximale a 255

    ledcWrite(coilNumber, dutyCycle);
  }
}

void Coil::updateCoil(){
  if(this->target_power > this->power + 10){
    this->power += 10;
  }
  else if(this->target_power < this->power -10){
    this->power -= 10;
  }
  else{
    this->power = this->target_power;
  }

  int dutyCycle = (int) ((this->power * 4095) / 999);
  if(dutyCycle > 4095) dutyCycle = 4095;  // Limiter la valeur maximale à 4095
  else if(dutyCycle < 0) dutyCycle = 0;  // Limiter la valeur minimale à 0
  if(this->direction && this->power < 10){
    digitalWrite(this->dir_pin, LOW);
  }
  else if(!this->direction && this->power < 10){
    digitalWrite(this->dir_pin, HIGH);
  }
  ledcWrite(this->coil_number, dutyCycle);
}