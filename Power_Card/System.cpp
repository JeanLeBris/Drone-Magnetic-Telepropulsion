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

  for(int i = 0; i < 8; i++){
    this->coils[i].PWM_pin = PWM_pins[i];
    this->coils[i].dir_pin = dir_pins[i];
    pinMode(this->coils[i].PWM_pin, OUTPUT);
    pinMode(this->coils[i].dir_pin, OUTPUT);
    ledcSetup(i, 10000, 12); // Fréquence de 10 kHz, résolution de 12 bits
    ledcAttachPin(this->coils[i].PWM_pin, i);
  }

  // this->joystick.minX = 1700;
  // this->joystick.maxX = 2000;
  // this->joystick.minY = 1700;
  // this->joystick.maxY = 2000;
  // this->joystick.centerX = 1000;
  // this->joystick.centerY = 1000;
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

    this->coils[0].power = data.substring(2, 6).toInt();
    this->coils[1].power = data.substring(7, 11).toInt();
    this->coils[2].power = data.substring(12, 16).toInt();
    this->coils[3].power = data.substring(17, 21).toInt();
    this->coils[4].power = data.substring(22, 26).toInt();
    this->coils[5].power = data.substring(27, 31).toInt();
    this->coils[6].power = data.substring(32, 36).toInt();
    this->coils[7].power = data.substring(37, 41).toInt();
  }
}

void System::printPower() {
  Serial.print(this->coils[0].power);
  Serial.print("-");
  Serial.print(this->coils[1].power);
  Serial.print("-");
  Serial.print(this->coils[2].power);
  Serial.print("-");
  Serial.print(this->coils[3].power);
  Serial.print("-");
  Serial.print(this->coils[4].power);
  Serial.print("-");
  Serial.print(this->coils[5].power);
  Serial.print("-");
  Serial.print(this->coils[6].power);
  Serial.print("-");
  Serial.println(this->coils[7].power);
}