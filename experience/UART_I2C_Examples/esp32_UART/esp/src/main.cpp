#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd display;

float esp_tx_data[2] = {50,60};
float esp_rx_data[2] = {0,0};

void setup() 
{
    Serial.begin(9600);
    Serial1.begin(115200, SERIAL_8N1, 16, 17);
    display.begin(16, 2);
    display.print("hello, world!");
}
void loop() 
{
  if(Serial1.available()>=16)
  {
    uint8_t esp_buffer[16];
    Serial1.readBytes(esp_buffer, 16);
    memcpy(&esp_rx_data[0],esp_buffer,sizeof(float));
    memcpy(&esp_rx_data[1],esp_buffer+sizeof(float),sizeof(float));
  }
  esp_tx_data[0] -=0.1;
  esp_tx_data[1] -=0.2;

  uint8_t tx_buffer[16];
  memcpy(tx_buffer,&esp_tx_data[0],sizeof(float));
  memcpy(tx_buffer+sizeof(float),&esp_tx_data[1],sizeof(float));
  Serial1.write(tx_buffer, 16);

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