#include <Arduino.h>
#include <cstdlib>
#include <BluetoothSerial.h>
#include "System.h"
#include "Coil.h"
#include "Constants.h"


#define NUM_COILS 8


System::System() {}

void System::begin() {
  /*Info...
	PWM_freq * 2^PWM_resolution cannot exceed the clock speed.
	PWM frequency and resolution are interdependent. The higher the PWM frequency, the lower the duty cycle resolution (and vice versa).
	clock source is at 80 MHz
	
	Pour F_PWM = 10KHz Resolution Max = 12 bits
  */
  this->joystick.begin();
  
  for(int i = 0; i < 8; i++){
    this->coils[i].begin(i);
  }
}

void System::updateData() {
  String data;
  data = Serial2.readStringUntil('\0');
  if(DEBUG){
    Serial.println(data);
  }
  if(data[0] == '0'){
    this->control_mode = 0;
  }
  else{
    this->control_mode = 1;

    // this->coils[0].direction = data[2] - 48;
    // this->coils[0].target_power = data.substring(3, 6).toInt();
    // this->coils[1].direction = data[7] - 48;
    // this->coils[1].target_power = data.substring(8, 11).toInt();
    // this->coils[2].direction = data[12] - 48;
    // this->coils[2].target_power = data.substring(13, 16).toInt();
    // this->coils[3].direction = data[17] - 48;
    // this->coils[3].target_power = data.substring(18, 21).toInt();
    // this->coils[4].direction = data[22] - 48;
    // this->coils[4].target_power = data.substring(23, 26).toInt();
    // this->coils[5].direction = data[27] - 48;
    // this->coils[5].target_power = data.substring(28, 31).toInt();
    // this->coils[6].direction = data[32] - 48;
    // this->coils[6].target_power = data.substring(33, 36).toInt();
    // this->coils[7].direction = data[37] - 48;
    // this->coils[7].target_power = data.substring(38, 41).toInt();
    
    this->coils[0].direction = data[2] - 48;
    this->coils[0].target_power = data.substring(3, 5).toInt();
    this->coils[1].direction = data[6] - 48;
    this->coils[1].target_power = data.substring(7, 9).toInt();
    this->coils[2].direction = data[10] - 48;
    this->coils[2].target_power = data.substring(11, 13).toInt();
    this->coils[3].direction = data[14] - 48;
    this->coils[3].target_power = data.substring(15, 17).toInt();
    this->coils[4].direction = data[18] - 48;
    this->coils[4].target_power = data.substring(19, 21).toInt();
    this->coils[5].direction = data[22] - 48;
    this->coils[5].target_power = data.substring(23, 25).toInt();
    this->coils[6].direction = data[26] - 48;
    this->coils[6].target_power = data.substring(27, 29).toInt();
    this->coils[7].direction = data[30] - 48;
    this->coils[7].target_power = data.substring(31, 33).toInt();
  }
}

void System::updatePowerFromJoystickTest(int num){
  int x = this->joystick.x;
  int y = this->joystick.y;

  /*if(this->coils[num].direction && x < -5){
    this->coils[num].direction = 0;
  }
  else if(!this->coils[num].direction && x > 5){
    this->coils[num].direction = 1;
  }*/
  
  this->coils[num].target_power = x;
}



void System::updatePowerFromJoystick() {
  int x = this->joystick.x;
  int y = this->joystick.y;

  // Calcul de l'angle du joystick par rapport à l'axe horizontal
  float angle = atan2(y, x);
  if (angle < 0) {
    angle += 2 * PI; // Ajustement pour obtenir un angle positif
  }

  // Calcul de la distance par rapport au centre du joystick
  int dist = (int)sqrt(pow(x, 2) + pow(y, 2));

  // Conversion de l'angle en position sur le cercle des 8 directions
  int joystickDirection = round(8 * angle / (2 * PI));
  // Si direction = 8, on le ramène à 0 pour une boucle complète
  if (joystickDirection == NUM_COILS) {
    joystickDirection = 0;
  }

  // Réinitialisation de la puissance cible de toutes les bobines
  for (int i = 0; i < NUM_COILS; i++) {
    this->coils[i].target_power = 0;
  }

  // Allumage de la bobine correspondante à la direction du joystick
  if (joystickDirection >= 0 && joystickDirection < NUM_COILS) {
    this->coils[joystickDirection].direction = 1;
    this->coils[joystickDirection].target_power = dist;
  }

  // Déterminer la bobine précédente et suivante
  int nextDirection = (joystickDirection + 1) % NUM_COILS;
  int prevDirection = (joystickDirection - 1 + NUM_COILS) % NUM_COILS;

  // Calcul de la distance en  tre le joystick et les deux directions adjacentes
  float distToNext = abs(angle - (2 * PI * nextDirection / NUM_COILS));
  float distToPrev = abs(angle - (2 * PI * prevDirection / NUM_COILS));

  // Ajouter une marge autour des angles pour n'allumer que la LED associée
  float margin = 0.1; // Marge en radians (environ 5.7 degrés)
  if (distToNext < margin) {
    float powerRatio = distToNext / margin;
    this->coils[nextDirection].target_power = dist * powerRatio;
  } else if (distToPrev < margin) {
    float powerRatio = distToPrev / margin;
    this->coils[prevDirection].target_power = dist * powerRatio;
  }

  // Appel de la fonction remanance
  remanence(joystickDirection);
}



/*void System::updatePowerFromJoystick(){
  int x = this->joystick.x;
  int y = this->joystick.y;

  
  int dist = (int) sqrt(pow(x, 2) + pow(y, 2));
  int cos6 = (int) dist * cos(PI / 6);
  int cos3 = (int) dist * cos(PI / 3);

  this->coils[0].target_power = 0;
  this->coils[1].target_power = 0;
  this->coils[2].target_power = 0;
  this->coils[3].target_power = 0;
  this->coils[4].target_power = 0;
  this->coils[5].target_power = 0;
  this->coils[6].target_power = 0;
  this->coils[7].target_power = 0;

  if(dist > 7){
    if(x > cos6){
      this->coils[0].direction = 1;
      this->coils[0].target_power = dist * 10;
      this->remanence(0);
    }
    else if(x < -cos6){
      this->coils[4].direction = 1;
      this->coils[4].target_power = dist * 10;
      this->remanence(4);
    }
    else if(y > cos6){
      this->coils[2].direction = 1;
      this->coils[2].target_power = dist * 10;
      this->remanence(2);
    }
    else if(y < -cos6){
      this->coils[6].direction = 1;
      this->coils[6].target_power = dist * 10;
      this->remanence(6);
    }
    else if(x > cos3 && y > cos3){
      this->coils[1].direction = 1;
      this->coils[1].target_power = dist * 10;
      this->remanence(1);
    }
    else if(x < -cos3 && y > cos3){
      this->coils[3].direction = 1;
      this->coils[3].target_power = dist * 10;
      this->remanence(3);
    }
    else if(x < -cos3 && y < -cos3){
      this->coils[5].direction = 1;
      this->coils[5].target_power = dist * 10;
      this->remanence(5);
    }
    else if(x > cos3 && y < -cos3){
      this->coils[7].direction = 1;
      this->coils[7].target_power = dist * 10;
      this->remanence(7);
    }
  }
}*/

void System::printPower() {
  Serial.print("B1:(dir= ");
  Serial.print(this->coils[0].direction);
  Serial.print(" pow= ");
  Serial.print(this->coils[0].power);
  Serial.print(") B2:(dir= ");
  Serial.print(this->coils[1].direction);
  Serial.print(" pow= ");
  Serial.print(this->coils[1].power);
  Serial.print(") B3:(dir= ");
  Serial.print(this->coils[2].direction);
  Serial.print(" pow= ");
  Serial.print(this->coils[2].power);
  Serial.print(") B4:(dir= ");
  Serial.print(this->coils[3].direction);
  Serial.print(" pow= ");
  Serial.print(this->coils[3].power);
  Serial.print(") B5:(dir= ");
  Serial.print(this->coils[4].direction);
  Serial.print(" pow= ");
  Serial.print(this->coils[4].power);
  Serial.print(") B6:(dir= ");
  Serial.print(this->coils[5].direction);
  Serial.print(" pow= ");
  Serial.print(this->coils[5].power);
  Serial.print(") B7:(dir= ");
  Serial.print(this->coils[6].direction);
  Serial.print(" pow= ");
  Serial.print(this->coils[6].power);
  Serial.print(") B8:(dir= ");
  Serial.print(this->coils[7].direction);
  Serial.print(" pow= ");
  Serial.print(this->coils[7].power);
  Serial.println(")");
}

void System::updateCoils() {
  float total_current = 0;
  for(int i = 0; i < 8; i++){
    total_current += ((abs(this->coils[i].target_power) * 24) / 99) / this->coils[i].resistance;
  }
  if(DEBUG){
    Serial.println(total_current);
  }

  if(total_current < 30){
    for(int i = 0; i < 8; i++){
      this->coils[i].setCoilPower();
    }
  }
  else{
    Serial.println("Threshold");
  }
}

void System::remanence(int activeCoil) {
  // Réduction de la puissance dans toutes les bobines qui ne sont pas actives
  for (int i = 0; i < NUM_COILS; i++) {
    if (i != activeCoil) {
      // Envoyer une faible puissance dans la direction opposée
      this->coils[i].direction = 0; // Direction opposée
      this->coils[i].target_power = -8; // Puissance faible
    }
  }
}
