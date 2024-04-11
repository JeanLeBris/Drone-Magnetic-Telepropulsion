#include <Arduino.h>
#include <cstdlib>
#include <BluetoothSerial.h>
#include "System.h"
#include "Coil.h"
#include "Constants.h"

System::System() {}

void System::begin() {
  /*Info...
	PWM_freq * 2^PWM_resolution cannot exceed the clock speed.
	PWM frequency and resolution are interdependent. The higher the PWM frequency, the lower the duty cycle resolution (and vice versa).
	clock source is at 80 MHz
	
	Pour F_PWM = 10KHz Resolution Max = 12 bits
  */
  
  // Initialisation des broches en mode de sortie

  // delay(2000);

  this->joystick.begin();

  for(int i = 0; i < 8; i++){
    // this->coils[i].PWM_pin = PWM_pins[i];
    // this->coils[i].dir_pin = dir_pins[i];
    // pinMode(this->coils[i].PWM_pin, OUTPUT);
    // pinMode(this->coils[i].dir_pin, OUTPUT);
    // ledcSetup(i, 10000, 12); // Fréquence de 10 kHz, résolution de 12 bits
    // ledcAttachPin(this->coils[i].PWM_pin, i);
    // this->coils[i].coil_number = i;
    this->coils[i].begin(i);
  }
}

/*
void Drone::getPosition() {
  String data = "\0";
  int delimiter_index = 0;
  int x_obj = 0;
  int y_obj = 0;
  if(Serial2.available()) {
    // data_rcvd = 0;
    // count = 0;
    // while(data_rcvd != '\0'){
      // Serial.flush();
      data = Serial2.readStringUntil('\0');   // read one byte from serial buffer and save to data_rcvd

      // if(data_rcvd == '1') digitalWrite(13, HIGH);  // switch LED On
      // if(data_rcvd == '0') digitalWrite(13, LOW);   // switch LED Off

      // if(data_rcvd == '\0'){
      //   data_rcvd = '\n';
      // }
      Serial.println(data);
      // delay(100);

      delimiter_index = data.indexOf(':');
      x_obj = data.substring(0, delimiter_index).toInt();
      y_obj = data.substring(delimiter_index + 1, data.length()).toInt();
      Serial.print(x_obj);
      Serial.print("-");
      Serial.println(y_obj);

      Serial2.println(data);

    //   data[count] = data_rcvd;
    //   count++;
    // }
    // Serial.println(data);
    // Serial.println("skip");
  }
  // Serial.println("Hello World!");
}
*/

void System::updateData() {
  String data;
  data = Serial2.readStringUntil('\0');
  Serial.println(data);
  if(data[0] == '0'){
    this->control_mode = 0;
  }
  else{
    this->control_mode = 1;

    this->coils[0].direction = data[2] - 48;
    this->coils[0].power = data.substring(3, 6).toInt();
    this->coils[1].direction = data[7] - 48;
    this->coils[1].power = data.substring(8, 11).toInt();
    this->coils[2].direction = data[12] - 48;
    this->coils[2].power = data.substring(13, 16).toInt();
    this->coils[3].direction = data[17] - 48;
    this->coils[3].power = data.substring(18, 21).toInt();
    this->coils[4].direction = data[22] - 48;
    this->coils[4].power = data.substring(23, 26).toInt();
    this->coils[5].direction = data[27] - 48;
    this->coils[5].power = data.substring(28, 31).toInt();
    this->coils[6].direction = data[32] - 48;
    this->coils[6].power = data.substring(33, 36).toInt();
    this->coils[7].direction = data[37] - 48;
    this->coils[7].power = data.substring(38, 41).toInt();
  }
}

void System::updatePowerFromJoystickTest(int num){
  int x = this->joystick.x;
  int y = this->joystick.y;

  if(this->coils[num].direction && x < -5){
    this->coils[num].direction = 0;
  }
  else if(!this->coils[num].direction && x > 5){
    this->coils[num].direction = 1;
  }

  this->coils[num].target_power = abs(x)*10;
}

void System::updatePowerFromJoystick(){
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
}

void System::printPower() {
  Serial.print(this->coils[0].direction);
  Serial.print(this->coils[0].power);
  Serial.print("-");
  Serial.print(this->coils[1].direction);
  Serial.print(this->coils[1].power);
  Serial.print("-");
  Serial.print(this->coils[2].direction);
  Serial.print(this->coils[2].power);
  Serial.print("-");
  Serial.print(this->coils[3].direction);
  Serial.print(this->coils[3].power);
  Serial.print("-");
  Serial.print(this->coils[4].direction);
  Serial.print(this->coils[4].power);
  Serial.print("-");
  Serial.print(this->coils[5].direction);
  Serial.print(this->coils[5].power);
  Serial.print("-");
  Serial.print(this->coils[6].direction);
  Serial.print(this->coils[6].power);
  Serial.print("-");
  Serial.print(this->coils[7].direction);
  Serial.println(this->coils[7].power);
}

void System::updateCoils() {
  float total_current = 0;
  for(int i = 0; i < 8; i++){
    total_current += ((abs(this->coils[i].target_power) * 24) / 999) / this->coils[i].resistance;
  }
  Serial.println(total_current);
  if(total_current < 30){
    for(int i = 0; i < 8; i++){
      this->coils[i].updateCoil();
    }
  }
  else{
    Serial.println("Threshold");
  }
}

void System::remanence(int coil_number){
  // for(int i = 0; i < 8; i++){
  //   if(i != coil_number){
  //     this->coils[i].direction = 0;
  //     this->coils[i].target_power = 50;
  //   }
  // }
}