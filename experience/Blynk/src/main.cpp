//############ DEFINE ############
// TEMPERATURE & HUMIDITY SENSOR
#define DHT_pin 15 

// MQ2 GAS SENSOR
#define MQ2_pin 25

// 7 SEGMENT DISPLAY
#define CLK 26
#define DIO 27

// LCD
#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

// OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0X3c

// LED RGB
#define R_PIN 13
#define G_PIN 12
#define B_PIN 14

// BLYNK
#define BLYNK_TEMPLATE_ID "TMPL6HoQLy9qR"
#define BLYNK_TEMPLATE_NAME "NaOH"
#define BLYNK_AUTH_TOKEN "0bRmBboY8zVRysSFiQswCMnsj4A_35Sr"



//############ LIBRARY ############
#include <Arduino.h>
#include <Wire.h>
#include "DHT.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32_SSL.h>
#include <TM1637.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"



//############
DHT dht(DHT_pin,DHT22);
TM1637 senven_segment(CLK, DIO);
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
Adafruit_SSD1306 oled_display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;


// VARIABLES & CONSTANTS
int R_pinValue, G_pinValue, B_pinValue, gas_state = 1;
float temperature, humidity;
char ssid[] = "Wokwi-GUEST";
char right_pass[] = "IOT", pass[] = "";  
bool Clock_state = true, pass_state = false;
String in_pass;


//############ SETUP ############
void setup() {
  Serial.begin(9600);
  // TIMER
  if (! rtc.begin())
  {
    Serial.println("Ko co sensor");
    while (1);
    rtc.adjust(DateTime(2025,4,21,9,12,20));    
  }

  // LED RGB
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  // TEMPERATURE & HUMIDITY SENSOR
  dht.begin();

  // MQ2 GAS SENSOR
  pinMode(MQ2_pin, OUTPUT);

  // BLYNK
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // 7 SEGMENT DISPLAY
  senven_segment.init();
  senven_segment.set(BRIGHT_TYPICAL);

  // LCD 
  lcd.init();
  lcd.backlight();

  // OLED
  oled_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled_display.clearDisplay();
  oled_display.setTextSize(1);
  oled_display.setTextColor(SSD1306_WHITE);
}



//############ MAIN FUNCTIONS ############
void loop() {
  // BLYNK
  Blynk.run();

  // HUMIDITY & TEMPERATURE
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  Blynk.virtualWrite(V1,temperature);
  Blynk.virtualWrite(V4,humidity);

  // MQ2 GAS SENSOR
  gas_state = digitalRead(MQ2_pin);
  Blynk.virtualWrite(V5, !gas_state);

  if (!pass_state)
  {
    // LCD
    lcd.clear();
    lcd.print("ENTER PASSWORD:");
    Blynk.virtualWrite(V0,0);
    Blynk.virtualWrite(V2,0);
    Blynk.virtualWrite(V3,0);
    R_pinValue = 0;
    G_pinValue = 0;
    B_pinValue = 0;
    if (in_pass == right_pass)
    {
      pass_state = true;
      lcd.clear();
      lcd.print("RIGHT :)))");
    }
  }
  else 
  {
  // LED RGB  
  digitalWrite(R_PIN, R_pinValue);
  digitalWrite(G_PIN, G_pinValue);
  digitalWrite(B_PIN, B_pinValue);
  }
  // OLED
  oled_display.clearDisplay();
  oled_display.setCursor(0,0);
  oled_display.println(humidity);
  oled_display.println(' ');
  oled_display.println(temperature);
  oled_display.display();
  
  // TIMER
  DateTime now = rtc.now();
  
  // SEVEN SEGMENT DISPLAY
  senven_segment.display(0, (now.hour() / 10) % 10);
  senven_segment.display(1, now.hour() % 10);
  senven_segment.display(2, (now.minute() / 10) % 10);
  senven_segment.display(3, now.minute() % 10);
  delay(500);
  senven_segment.point(Clock_state);
  Clock_state = !Clock_state;
  delay(500);
}

//############
BLYNK_WRITE(V0)
{
  R_pinValue = param.asInt();
}
BLYNK_WRITE(V2)
{
  G_pinValue = param.asInt();
}
BLYNK_WRITE(V3)
{
  B_pinValue = param.asInt();
}
BLYNK_WRITE(V6)
{
  in_pass = param.asString();
}
//############