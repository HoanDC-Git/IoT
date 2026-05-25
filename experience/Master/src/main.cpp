//#################### DEFINE ####################
#define SIZE_OF_DATA 8
#define SlaveAddress 0x11
#define DHT_pin 5



//#################### LIBRARY ###################

#include <Arduino.h>
#include <Wire.h>


//############ VARIABLES & CONSTANTS #############
float h, t, data1, data2, send_data1 = 0, send_data2 = 0;


//#################### SETUP #####################
void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void Request_Data()
{
  Wire.requestFrom(SlaveAddress,SIZE_OF_DATA);
  if (Wire.available() >= SIZE_OF_DATA)
  {
    Wire.readBytes((byte*)&data1, sizeof(data1));
    Wire.readBytes((byte*)&data2, sizeof(data2));
  }
}

void Transmit_Data()
{
  Wire.beginTransmission(SlaveAddress);
  Wire.write((byte*)&send_data1, sizeof(send_data1));
  Wire.write((byte*)&send_data2, sizeof(send_data2));
  Wire.endTransmission();
}


void loop() {
  Request_Data();
  Serial.print("Send:");
  Serial.print(send_data1);
  Serial.print('|');
  Serial.println(send_data2);

  Serial.print("Received:");
  Serial.print(data1);
  Serial.print('|');
  Serial.println(data2);
  Serial.println("--------");

  Transmit_Data();
  send_data1 += 0.5;
  send_data2 += 1;
  if (send_data1 > 10000) send_data1 = 0;
  if (send_data2 > 10000) send_data2 = 0;
  delay(1000); 
}