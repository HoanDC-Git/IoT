//############ DEFINE ############
// LED RGB
#define R_PIN 13
#define G_PIN 12
#define B_PIN 14

// 7 SEGMENT DISPLAY
#define CLK 25
#define DIO 26

// HX711
// #define SCK 15
// #define DT 2

// LCD
#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

// OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0X3c

//TEMPERATURE & HUMIDITY SENSOR
#define DHT_pin 5  

//############ LIBRARY ############
#include <Arduino.h>
#include <Wire.h>
#include <TM1637.h>
#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <Adafruit_SSD1306.h>

//############
TM1637 senven_segment(CLK, DIO);
HX711 scale;
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
DHT dht(DHT_pin,DHT22);
Adafruit_SSD1306 oled_display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

//############ SETUP ############
void setup() {
  Serial.begin(9600);
  // LED RGB
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  // 7 SEGMENT DISPLAY
  senven_segment.init();
  senven_segment.set(BRIGHT_TYPICAL);

  // HX711
  scale.begin(2, 15);
  scale.set_scale(0.42);    //0.42

  // LCD 
  lcd.init();
  lcd.backlight();

  // OLED
  oled_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled_display.clearDisplay();
  oled_display.setTextSize(2);
  oled_display.setTextColor(SSD1306_WHITE);
  
  // TEMPERATURE & HUMIDITY SENSOR
  dht.begin();
}

//############ VARIABLES & CONSTANTS ############
unsigned int counter = 0;
float weight = 0, temperature, humidity;

//############ MAIN FUNCTION ############
void loop() {
  counter++;
  if (counter == 10000) {
    counter = 0;
  }

  // LED RGB
  if (counter % 3 == 0)
  {
    digitalWrite(B_PIN, LOW);
    digitalWrite(R_PIN, HIGH);
  }
  else if (counter % 3 == 1)
  {
    digitalWrite(R_PIN, LOW);
    digitalWrite(G_PIN, HIGH);
  }
  else
  {
    digitalWrite(G_PIN, LOW);
    digitalWrite(B_PIN, HIGH);
  }

  // SEVEN SEGMENT DISPLAY
  senven_segment.display(0, (counter / 1000) % 10);
  senven_segment.display(1, (counter / 100) % 10);
  senven_segment.display(2, (counter / 10) % 10);
  senven_segment.display(3, counter % 10);

  // HUMIDITY & TEMPERATURE
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.print(temperature);
  
  // OLED
  oled_display.clearDisplay();
  oled_display.setCursor(0,0);
  oled_display.println(humidity);
  oled_display.println(' ');
  oled_display.println(temperature);
  oled_display.display();
  
  // SCALE (HX711) & LCD
  weight = scale.get_units()/1000;
  lcd.clear();
  lcd.print(counter);
  delay(300);
  lcd.clear();
  lcd.print(weight);
  lcd.print("(kg)");
  delay(700);
}