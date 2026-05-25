#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f,20,4);  

float tx_data[2];
float rx_data[2];

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  //Khởi tạo IIC --> esp
  Wire.begin();
}
void loop()
{

  tx_data[0] +=0.1;
  tx_data[1] +=0.2;
  uint8_t buffer[8];
  memcpy(buffer,&tx_data[0],sizeof(float));
  memcpy(buffer+sizeof(float),&tx_data[1],sizeof(float));
  
  Wire.beginTransmission(0x11); //truy xuất tới slave có địa chỉ 0x11
  Wire.write(buffer, sizeof(buffer)); //Ghi dữ liệu vào slave
  Wire.endTransmission(); //Kết thúc truyền dữ liệu

  Wire.requestFrom(0x11, 8); //Yêu cầu nhận dữ liệu từ slave
  if(Wire.available() == 8) //Kiểm tra xem có dữ liệu nhận được không
  {
    uint8_t rx_buffer[8];
    for (int i = 0; i < 8; i++) 
    {
      if(Wire.available()) 
      {
        rx_buffer[i] = Wire.read();
      }else
      {
        rx_buffer[i] = 0 ;
      }
    }
    memcpy(&rx_data[0], rx_buffer, sizeof(float));
    memcpy(&rx_data[1], rx_buffer+sizeof(float), sizeof(float));
  }
  else
  {
    while(Wire.available()) 
    {
      Wire.read();
    }
  }

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