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
  //pinMode(this->PWM_pin, OUTPUT);
  pinMode(this->dir_pin, OUTPUT);
  
  ledcSetup(i, 10000, 12); // Fréquence de 10 kHz, résolution de 12 bits
  ledcAttachPin(this->PWM_pin, i);
  this->coil_number = i;
  this->power = 0;

  ledcWrite(this->coil_number, 0);

  /*this->direction = 0;
  digitalWrite(this->dir_pin, LOW);
  delay(1000);
  this->direction = 1;
  digitalWrite(this->dir_pin, HIGH);
  delay(1000);
  ledcWrite(this->coil_number, 0);*/
  this->direction = 1;
  digitalWrite(this->dir_pin, HIGH);
  this->power = 0;
  this->target_power = 0;
}

void Coil::setCoilPower(){
  
  if (this->power == 0 && this->target_power != 0) {
            //this->direction = (this->target_power > 0) ? 1 : 0;
            delay(50);
            
            digitalWrite(this->dir_pin, direction);
  }
  
  if(this->target_power > this->power){
    this->power ++;
  }
  else if(this->target_power < this->power){
    this->power --;
  }


  int dutyCycle = (int) (abs((this->power * 3900)) / 99); //3900 et pas 4095 car l'ir2110 doit toujour avoir un pwm en entrer
  
  if(dutyCycle > 3900) dutyCycle = 3900;  // Limiter la valeur maximale à 4095
  else if(dutyCycle < 0) dutyCycle = 0;  // Limiter la valeur minimale à 0
  
  ledcWrite(this->coil_number, dutyCycle);
  delayMicroseconds(100);
}
