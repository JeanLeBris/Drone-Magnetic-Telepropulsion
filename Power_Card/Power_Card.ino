#include <Arduino.h>
#include <cstdlib>
#include <BluetoothSerial.h>
#include "Joystick.h"
#include "Coil.h"
#include "System.h"
#include "Constants.h"






// void setup() {
//   Serial.begin(115200);
//   pinMode(2,OUTPUT);
// }

// void loop() {
//   digitalWrite(2, HIGH); //On allume la led bleue
//   Serial.println("Hello World!");
//   delay(500); //On attend pendant 500 ms
//   digitalWrite(2, LOW); //On eteinds la led bleue
//   delay(500); //On attend pendant 500 ms
// }








// CoilControl bobine;

// Drone drone;

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
  // pinMode(8, INPUT_PULLUP); // set push button pin as input
  // pinMode(13, OUTPUT);      // set LED pin as output
  // digitalWrite(13, LOW);    // switch off LED pin
  system1.begin(); // initialize system
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);       // initialize UART with baud rate of 9600 bps
}

void loop() {
  // if (system1.joystick.SerialBT.available()) {
  //   // if(Serial2.available()) {

  //     data = system1.joystick.SerialBT.readStringUntil('\n');
  //     if (data.length() > 0) {
  //       int separatorIndex = data.indexOf(',');
  //       if (separatorIndex != -1) {
  //         String xStr = data.substring(0, separatorIndex);
  //         String yStr = data.substring(separatorIndex + 1);
  //         x = xStr.toInt();
  //         y = yStr.toInt();



  //         // Utiliser les valeurs x et y pour contrôler le dispositif
  //         // Par exemple, contrôler des moteurs, des servomoteurs, etc.

  //         // Serial.print("X: ");
  //         // Serial.print(x);
  //         // Serial.print("\tY: ");
  //         // Serial.println(y);
  //       }
  //     }

  //     // system1.updateData();
  //     system1.control_mode = 0;
  //     if(system1.control_mode){
  //       // auto mode
  //     }
  //     else{
  //       // manual mode
  //       // system1.joystick.updateJoystick();
  //       system1.joystick.updateJoystickMobile(x, y);
  //       system1.updatePowerFromJoystickTest(6);
  //     }
  //     system1.printPower();
  //     system1.updateCoils();
  //   // }
  // }

  for(int i = 0; i < 999; i++){
    system1.coils[6].power = i;
    system1.coils[6].direction = 0;
    system1.coils[6].updateCoil();
    Serial.print(system1.coils[6].power);
    Serial.print(" - ");
    Serial.println(system1.coils[6].direction);
    delay(5);
  }
  for(int i = 999; i > 0; i--){
    system1.coils[6].power = i;
    system1.coils[6].direction = 1;
    system1.coils[6].updateCoil();
    Serial.print(system1.coils[6].power);
    Serial.print(" - ");
    Serial.println(system1.coils[6].direction);
    delay(5);
  }
}





// void setup() {
//   Serial.begin(115200);
//   SerialBT.begin("ESP32_BT"); // Nom du périphérique Bluetooth
// }

// void loop() {
//   if (SerialBT.available()) {
//     String data = SerialBT.readStringUntil('\n');
//     if (data.length() > 0) {
//       int separatorIndex = data.indexOf(',');
//       if (separatorIndex != -1) {
//         String xStr = data.substring(0, separatorIndex);
//         String yStr = data.substring(separatorIndex + 1);
//         int x = xStr.toInt();
//         int y = yStr.toInt();

//         // Utiliser les valeurs x et y pour contrôler le dispositif
//         // Par exemple, contrôler des moteurs, des servomoteurs, etc.
//         Serial.print("X: ");
//         Serial.print(x);
//         Serial.print("\tY: ");
//         Serial.println(y);
//       }
//     }
//   }
// }