#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
LiquidCrystal_I2C lcd(0x3f,20,4);  

float tx_data[2];
float rx_data[2];

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(115200);
  lcd.init();
  lcd.backlight();
}
void loop()
{
  if(mySerial.available()>=16)
  {
    uint8_t uno_buffer[16];
    mySerial.readBytes(uno_buffer, 16);
    memcpy(&rx_data[0],uno_buffer,sizeof(float));
    memcpy(&rx_data[1],uno_buffer+sizeof(float),sizeof(float));
  }

  tx_data[0] +=0.1;
  tx_data[1] +=0.2;
  uint8_t buffer[16];
  memcpy(buffer,&tx_data[0],sizeof(float));
  memcpy(buffer+sizeof(float),&tx_data[1],sizeof(float));
  mySerial.write(buffer, 16);

  lcd.clear();
  lcd.print("TX:");
  lcd.print(tx_data[0]);
  lcd.setCursor(8,0);
  lcd.print("|");
  lcd.print(tx_data[1]);

  lcd.setCursor(0,1);
  lcd.print("RX:");
  lcd.print(rx_data[0]);
  lcd.setCursor(8,1);
  lcd.print("|");
  lcd.print(rx_data[1]);


  digitalWrite(13,HIGH);delay(100);
  digitalWrite(13,LOW);delay(100);

}