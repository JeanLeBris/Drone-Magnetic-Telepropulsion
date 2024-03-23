#include <Arduino.h>
#include "CoilControl.h"






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






#define PWM_1 32
#define PWM_2 33
#define PWM_3 25
#define PWM_4 26
#define PWM_5 27
#define PWM_6 14
#define PWM_7 12
#define PWM_8 13

#define Dir_1 15
#define Dir_2 2
#define Dir_3 0
#define Dir_4 4
#define Dir_5 5
#define Dir_6 18
#define Dir_7 19
#define Dir_8 23

#define RX2 16
#define TX2 17

#define SDA 21
#define SCL 22

#define Btn 36
#define Y 39
#define X 34

CoilControl bobine;

String data = "\0";
char data_rcvd = 0;
int count = 0;
int x_obj = 0;
int y_obj = 0;
int delimiter_index = 0;
int verif = 0;

void setup() {
  // pinMode(8, INPUT_PULLUP); // set push button pin as input
  // pinMode(13, OUTPUT);      // set LED pin as output
  // digitalWrite(13, LOW);    // switch off LED pin
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);       // initialize UART with baud rate of 9600 bps
  bobine.begin(); // initialize coil
}

void loop() {
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






/*
void loop() {
  // put your main code here, to run repeatedly:
  int sensorValueX = analogRead(0);
  int sensorValueY = analogRead(1);

  // sensorValueX = map(sensorValueX, 396,1023,-100,100);
  // sensorValueY = map(sensorValueY, 410,1023,-100,100);

  // if(sensorValueX < sensorMinX){
  //   sensorMinX = sensorValueX;
  // }
  // if(sensorValueX > sensorMaxX){
  //   sensorMaxX = sensorValueX;
  // }
  // if(sensorValueY < sensorMinY){
  //   sensorMinY = sensorValueY;
  // }
  // if(sensorValueY > sensorMaxY){
  //   sensorMaxY = sensorValueY;
  // }

  // sensorCenterX = (sensorMinX + sensorMaxX) / 2;
  // sensorCenterY = (sensorMinY + sensorMaxY) / 2;
  Serial.print("The X and Y coordinate is:");
  Serial.print(sensorValueX, DEC);
  Serial.print(",");
  Serial.print(sensorValueY, DEC);
  // Serial.print(" \tThe min and max of X and Y coordinate are:\t");
  // Serial.print(sensorMinX, DEC);
  // Serial.print(",");
  // Serial.print(sensorMaxX, DEC);
  // Serial.print(",");
  // Serial.print(sensorMinY, DEC);
  // Serial.print(",");
  // Serial.print(sensorMaxY, DEC);
  // Serial.print(" \tThe center of X and Y coordinate are:\t");
  // Serial.print(sensorCenterX, DEC);
  // Serial.print(",");
  // Serial.println(sensorCenterY, DEC);

  // if(sensorValueX > sensorCenterX){
  //   sensorValueX = ((sensorValueX - sensorCenterX) * 100) / (sensorMaxX - sensorCenterX);
  // }
  // else{
  //   sensorValueX = ((sensorValueX - sensorCenterX) * 100) / (sensorCenterX - sensorMinX);
  // }
  // if(sensorValueY > sensorCenterY){
  //   sensorValueY = -((sensorValueY - sensorCenterY) * 100) / (sensorMaxY - sensorCenterY);
  // }
  // else{
  //   sensorValueY = -((sensorValueY - sensorCenterY) * 100) / (sensorCenterY - sensorMinY);
  // }
  sensorValueX = MapAdjustment(sensorValueX, sensorCenterX, sensorMinX, sensorMaxX);
  sensorValueY = MapAdjustment(sensorValueY, sensorCenterY, sensorMinY, sensorMaxY);
  Serial.print(" \tThe X and Y coordinate is:");
  // Serial.print(sensorValueX - sensorCenterX, DEC);
  // Serial.print(",");
  // Serial.println(sensorValueY - sensorCenterY, DEC);
  Serial.print(sensorValueX, DEC);
  Serial.print(",");
  Serial.println(sensorValueY, DEC);
  
  Serial.println("Hello World!");
  delay(200);
}

int MapAdjustment(int value, int center, int min, int max){
  if(value > center){
    value = ((value - center) * 100) / (max - center);
  }
  else{
    value = ((value - center) * 100) / (center - min);
  }
  return value;
}
*/





/*
void loop() {
  // Exemple : Définir un courant de 1A pour la bobine 0
  //bobine.setCoilCurrent(0, 1.0);
  //delay(1000); // Attendre une seconde
  
  // Exemple : Définir un courant de 0.5A pour la bobine 1
  bobine.setCoilCurrent(1, 0.5);
  //delay(1000); // Attendre une seconde
}
 */