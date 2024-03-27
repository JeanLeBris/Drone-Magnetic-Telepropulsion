#include "CoilControl.h"
#include "Constants.h"
#include <driver/ledc.h>

CoilControl::CoilControl() {}

void CoilControl::begin() {
 
  /*Info...
	PWM_freq * 2^PWM_resolution cannot exceed the clock speed.
	PWM frequency and resolution are interdependent. The higher the PWM frequency, the lower the duty cycle resolution (and vice versa).
	clock source is at 80 MHz
	
	Pour F_PWM = 10KHz Resolution Max = 12 bits
  */
  
  // Initialisation des broches en mode de sortie
  for(int i = 0; i < 8; i++) {
    pinMode(pwmPins[i], OUTPUT);
    ledcSetup(i, 10000, 12); // Fréquence de 10 kHz, résolution de 12 bits
    ledcAttachPin(pwmPins[i], i);
  }
}

void CoilControl::setCoilCurrent(int coilNumber, float current) {
  if(coilNumber >= 0 && coilNumber < 8) {
    float voltage = current * resistances[coilNumber];
    int dutyCycle = (voltage / 24) * 4095; // L'alimentation est de 24V

    if(dutyCycle > 4095) dutyCycle = 4095; // Limiter à la valeur maximale a 255

    ledcWrite(coilNumber, dutyCycle);
  }
}
