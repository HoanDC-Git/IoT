#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd display;

float esp_tx_data[2] = {50,60};
float esp_rx_data[2] = {0,0};

#define SDA_PIN 18
#define SCL_PIN 19
#define SlaveAddress 0x11

void receiveEvent(int howMany) 
{
  if (howMany == 8) 
  {
    uint8_t rx_buffer[8];
    for (int i = 0; i < 8; i++) 
    {
      if(Wire1.available()) 
      {
        rx_buffer[i] = Wire1.read();
      }else
      {
        rx_buffer[i] = 0 ;
      }
    }
    memcpy(&esp_rx_data[0], rx_buffer, sizeof(float));
    memcpy(&esp_rx_data[1], rx_buffer+sizeof(float), sizeof(float));
  }
  else
  {
    while(Wire1.available()) 
    {
      Wire1.read();
    }
  }
}
void requestEvent() 
{
  uint8_t tx_buffer[8];
  memcpy(tx_buffer, &esp_tx_data[0], sizeof(float));
  memcpy(tx_buffer+sizeof(float), &esp_tx_data[1], sizeof(float));
  Wire1.write(tx_buffer, 8);

  esp_tx_data[0] -=0.1;
  esp_tx_data[1] -=0.3;
  
}
void setup() 
{
    Serial.begin(9600);
    display.begin(16, 2);
    display.print("hello, world!");
    Wire1.begin(uint8_t(SlaveAddress), SDA_PIN, SCL_PIN,100000);
    Wire1.onRequest(requestEvent);
    Wire1.onReceive(receiveEvent);
}
void loop() 
{
  

  display.clear();
  display.print("RX:");
  display.print(esp_rx_data[0]);
  display.setCursor(8,0);
  display.print("|");
  display.print(esp_rx_data[1]);

  display.setCursor(0,1);
  display.print("TX:");
  display.print(esp_tx_data[0]);
  display.setCursor(8,1);
  display.print("|");
  display.print(esp_tx_data[1]);
  
  delay(200);

}