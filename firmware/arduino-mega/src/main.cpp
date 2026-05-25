//#################### DEFINE ####################
#define SlaveAddress 0x11
// ##### SENSOR #####
// PIR MOTION SENSOR
#define PIR_PIN 31                  // DONE

// TEMPERATURE & HUMIDITY SENSOR
#define DHT_PIN 41                  // DONE

// ULTRASONIC
#define TRIG_PIN 51
#define ECHO_PIN 53

// MQ2 GAS SENSOR  
#define MQ2_PIN A7                  //DONE

// LDR PHOTORESISTOR
#define LDR_PIN A0                  // DONE


// ##### ACTUATOR #####
// RELAY
#define AIR_CONDI_RELAY_PIN 12    //DONE
#define LED_RELAY_PIN 8

// LED RGB
#define R_PIN 2
#define G_PIN 4
#define B_PIN 6

// BUZZER
#define BUZZER_PIN 11               // DONE
#define BUZZER_FRE 3000

// // OLED                               // DONE
// #define OLED_WIDTH 128
// #define OLED_HEIGHT 64
// #define OLED_RESET -1
// #define OLED_ADDR 0X3c

//#################### LIBRARY ###################
#include <math.h>
#include <Arduino.h>
#include <Wire.h>
#include "DHT.h"
// #include <Adafruit_SSD1306.h>
// #include "RTClib.h"



//############ VARIABLES & CONSTANTS #############
struct MegaData
{
  float temperature;
  float humidity;
  float lux;
  float ppm;
  float distance;
  bool air_condi_status;
  bool pir_status;
  bool timeup;
  byte padding[1]; 
};

struct EspData
{
  float hightemperature;
  float lowtemperature;

  uint8_t RedValue;
  uint8_t GreenValue;
  uint8_t BlueValue;
  bool ProtectMode;

  bool GasBuzzerOff;
  bool AutoMode;
  bool RGBOn;
  bool LedOn;
  
  uint8_t OnTimer;
  byte padding[3];
};


MegaData DataFromMega;
EspData DataFromEsp;
float GAMMA = 0.7, RL10 = 50, MQ2_Ro = 10.0;
bool check_human = false, check_gas = false, ledstatus = false;
const float MQ2_RL = 10.0, MQ2_Vc = 5.0, MQ2_m = -0.473, MQ2_b = 1.413; // MQ2: resistance, voltage, slope, intercept

const unsigned char PROGMEM thermometer_icon[] = {
0,0,7,224,0,0,0,0,31,240,0,0,0,0,56,56,0,0,0,0,48,24,0,0,0,0,48,8,0,0,0,0,48,8,0,0,0,0,48,8,0,0,0,0,48,12,0,0,0,0,48,12,0,0,0,0,48,31,128,0,0,0,48,12,0,0,0,0,48,12,0,0,0,0,48,15,128,0,0,0,48,31,128,0,0,0,48,12,0,0,0,0,48,12,0,0,0,0,48,15,128,0,0,0,48,15,128,0,0,0,48,12,0,0,0,0,48,12,0,0,0,0,48,15,128,0,0,0,48,31,128,0,0,0,63,252,0,0,0,0,63,252,0,0,0,0,48,15,192,0,0,0,48,15,128,0,0,0,48,12,0,0,0,0,112,14,0,0,0,0,224,3,0,0,0,1,128,1,128,0,0,3,128,0,192,0,0,3,0,0,192,0,0,6,0,0,96,0,0,6,0,0,96,0,0,6,0,0,96,0,0,6,0,0,48,0,0,6,0,0,48,0,0,6,0,0,48,0,0,6,0,0,112,0,0,6,0,0,96,0,0,7,0,0,96,0,0,3,0,0,224,0,0,3,128,0,192,0,0,1,192,1,128,0,0,0,224,7,0,0,0,0,124,62,0,0,0,0,63,252,0,0,0,0,7,224,0,0
};
const int ICON_WIDTH_SMALL = 48;
const int ICON_HEIGHT_SMALL = 48;


//############
// Adafruit_SSD1306 oled_display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
// RTC_DS1307 rtc;
DHT dht(DHT_PIN,DHT11);



//################### PROCEDUCE ##################
void Request_Data()
{
  Wire.requestFrom(SlaveAddress,sizeof(EspData));
  if (Wire.available() == sizeof(EspData))
  {
    Wire.readBytes((byte*)&DataFromEsp, sizeof(EspData));
  }
}

void Transmit_Data()
{
  Wire.beginTransmission(SlaveAddress);
  Wire.write((byte*)&DataFromMega, sizeof(MegaData));
  Wire.endTransmission();
}

void setLedColor(int r, int g, int b) {
  // Limit values in the range 0-255
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  // Write PWM values
  analogWrite(R_PIN,r);
  analogWrite(G_PIN,g);
  analogWrite(B_PIN,b);
}

//#################### SETUP #####################
void setup() {
  Serial.begin(9600);

  // I2C
  Wire.begin();

  // ##### SENSOR #####
  // TIMER RTC
  // rtc.begin();


  // PIR MOTION SENSOR
  pinMode(PIR_PIN, INPUT);

  // TEMPERATURE & HUMIDITY SENSOR
  dht.begin();

  // ULTRASONIC
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


  // ##### ACTUATOR #####
  // LED & LED RGB

  // RELAY
  pinMode(AIR_CONDI_RELAY_PIN, OUTPUT);
  pinMode(LED_RELAY_PIN, OUTPUT);

  // BUZZER
  pinMode(BUZZER_PIN, OUTPUT);
  //   // OLED
  // oled_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  // oled_display.clearDisplay();
  // oled_display.setTextSize(2);
  // oled_display.setTextColor(SSD1306_WHITE);

  //  DEFAULT
  DataFromMega.timeup = 0;
}


//#################### MAIN #####################
void loop() {
  // // TIMER RTC
  // DateTime now = rtc.now();
  Request_Data();
  // TEMPERATURE & HUMIDITY SENSOR
  DataFromMega.temperature = dht.readTemperature();
  DataFromMega.humidity = dht.readHumidity();
  if (DataFromMega.temperature >= DataFromEsp.hightemperature)
  {
    if (DataFromMega.air_condi_status == false)
    {
      DataFromMega.air_condi_status = true;
      digitalWrite(AIR_CONDI_RELAY_PIN,HIGH);
    }
  } 
  if (DataFromMega.temperature <= DataFromEsp.lowtemperature)
  {
    if (DataFromMega.air_condi_status == true)
    {
      DataFromMega.air_condi_status = false;
      digitalWrite(AIR_CONDI_RELAY_PIN,LOW);
    }
  }  
  
  // LDR PHOTORESISTOR
  int analogValue = analogRead(LDR_PIN);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  DataFromMega.lux = roundf(lux*10.0f)/10.0f;

  // MQ2 GAS SENSOR
  float Vrl = analogRead(MQ2_PIN) * MQ2_Vc / 1023.0;
  float Rs = (MQ2_Vc - Vrl) * MQ2_RL / Vrl;
  float ratio = Rs / MQ2_Ro;
  DataFromMega.ppm = pow(10, (log10(ratio) - MQ2_b) / MQ2_m);
  if (!DataFromEsp.GasBuzzerOff) check_gas = false;
  if (DataFromMega.ppm >= 0.5) check_gas = true;
  
  // PIR MOTION SENSOR
  DataFromMega.pir_status = digitalRead(PIR_PIN);
  if (DataFromEsp.ProtectMode == true)
  {
    if (DataFromMega.pir_status == true) check_human = true;
  }
  else check_human = false;

  //ULTRASONIC
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  int duration = pulseIn(ECHO_PIN, HIGH);
  DataFromMega.distance = duration / 58.2;

  Transmit_Data();
  if (check_human == true || check_gas == true) tone(BUZZER_PIN, BUZZER_FRE,200);
  
  if (DataFromEsp.AutoMode)
  {
    if (DataFromMega.lux <= 300) digitalWrite(LED_RELAY_PIN, HIGH);
    else if (DataFromMega.lux > 5000) digitalWrite(LED_RELAY_PIN, LOW);
  }
  else
  {
    if (DataFromEsp.LedOn == true)
    {
      digitalWrite(LED_RELAY_PIN, HIGH);
    }
    else digitalWrite(LED_RELAY_PIN, LOW);
  }
  Serial.println("----------");
  if (DataFromEsp.RGBOn)
  {
    setLedColor(DataFromEsp.RedValue, DataFromEsp.GreenValue, DataFromEsp.BlueValue);
  }
  else setLedColor(0,0,0);
  Serial.print(DataFromMega.ppm);
  Serial.print(" | ");
  Serial.print(DataFromEsp.GreenValue);
  Serial.print(" | ");
  Serial.println(DataFromEsp.BlueValue);

  // //OLED
  // if (count <= 7)
  // {
  //   oled_display.setTextSize(4);
  //   oled_display.setCursor(3,5);
  //   oled_display.setTextColor(SSD1306_WHITE);
  //   if (now.hour() < 10) oled_display.print('0'); 
  //   oled_display.print(now.hour());
  //   oled_display.print(':');
  //   if (now.minute() < 10) oled_display.print('0'); 
  //   oled_display.println(now.minute());
  //   oled_display.setTextSize(2);
  //   if (now.day() < 10) oled_display.print('0'); 
  //   oled_display.print(now.day());
  //   oled_display.print(".");
  //   if (now.month() < 10) oled_display.print('0'); 
  //   oled_display.print(now.month());
  //   oled_display.print(".");
  //   oled_display.print(now.year());
  // }
  // else
  // {
  //   // --- Display Temperature ---
  //   int currentY = 10; // Starting Y position
  //   int iconX = 0;    // Icon X position
  //   //int textX = iconX + ICON_WIDTH_SMALL + 5; // Text X position, 5px away from icon
  //   oled_display.drawBitmap(iconX, currentY, thermometer_icon, ICON_WIDTH_SMALL, ICON_HEIGHT_SMALL, SSD1306_WHITE);
  //   oled_display.setCursor(50, 30);
  //   oled_display.setTextSize(3);
  //   if (!isnan(DataFromMega.temperature)) oled_display.print(int(DataFromMega.temperature));
  //   else                                  oled_display.print("__");
  //   oled_display.print(".C");
  // }
  // oled_display.display();
  // count = (count + 1) % 10;
  delay(1000);
}
