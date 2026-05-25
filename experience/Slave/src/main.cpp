#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define SlaveAddress 0x11

float data1, data2, send_data1 = 0, send_data2 = 0;

void receiveEvent(int howMany)
{
  if (Wire.available() >= 8)
  {
    Wire.readBytes((byte*)&data1, sizeof(data1));
    Wire.readBytes((byte*)&data2, sizeof(data2));
  }
  else
  {
    while (Wire.available())
    {
      Wire.read();
    }
  }
}

void requestEvent()
{
  // truyen
  Wire.write((byte*)&send_data1, sizeof(send_data1));
  Wire.write((byte*)&send_data2, sizeof(send_data2));
  send_data1 += 0.1;
  send_data2 += 0.2;
}

void setup() {
  Serial.begin(9600);
  Wire.begin(SlaveAddress);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  Serial.print("Send:");
  Serial.print(send_data1);
  Serial.print('|');
  Serial.println(send_data2);

  Serial.print("Received:");
  Serial.print(data1);
  Serial.print('|');
  Serial.println(data2);
  Serial.println("--------");

  delay(1000);
}