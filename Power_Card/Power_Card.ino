#include <Arduino.h>
#include <cstdlib>
#include <BluetoothSerial.h>
#include "Joystick.h"
#include "Coil.h"
#include "System.h"
#include "Constants.h"

System system1;

String data = "\0";
char data_rcvd = 0;
int count = 0;
int x = 0;
int y = 0;
int x_obj = 0;
int y_obj = 0;
int delimiter_index = 0;
int verif = 0;

void setup() {
  system1.begin(); // initialize system
  Serial.begin(500000);
  // Serial2.begin(500000, SERIAL_8N1, RX2, TX2);       // initialize UART with baud rate of 115200 bps
}

void loop() {
  if (system1.joystick.SerialBT.available()) {
    // if(Serial2.available()) {

      data = system1.joystick.SerialBT.readStringUntil('\n');
      if (data.length() > 0) {
        int separatorIndex = data.indexOf(',');
        if (separatorIndex != -1) {
          String xStr = data.substring(0, separatorIndex);
          String yStr = data.substring(separatorIndex + 1);
          x = xStr.toInt();
          y = yStr.toInt();
          /*Serial.print("X: ");
          Serial.print(x);
          Serial.print("\tY: ");
          Serial.println(y);*/
        }
      }

      // system1.updateData();
      system1.control_mode = 0;
      if(system1.control_mode){
        // auto mode
      }
      else{
        // manual mode
        system1.joystick.updateJoystickMobile(x, y);
        system1.updatePowerFromJoystick();
      }
      if(DEBUG){
        // system1.printPower();
      }
      system1.updateCoils();
    // }
  }
}
