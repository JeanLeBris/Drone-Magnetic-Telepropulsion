#include "Drone.h"
#include "Constants.h"

Drone::Drone() {}

void Drone::getPosition() {
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
