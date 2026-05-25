//############ DEFINE ############
// ULTRASONIC
#define pin_TRIG 19
#define pin_ECHO 18

// GAS
#define pin_AUOT 32


//############ LIBRARY ############
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "ThingSpeak.h"

//############
WiFiClient client;
unsigned long myChannelNumber = 2937532;
const char * myWriteAPIKey = "EBCOCAH0FF0N6M4P";



// VARIABLES & CONSTANTS
int duration, gas_value;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

//############ SETUP ############
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  if (WiFi.status() != WL_CONNECTED)
  {
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print('.');
      delay(1000);
    }
  }
  ThingSpeak.setField(1,duration);

  // ULTRASONIC
  pinMode(pin_TRIG, OUTPUT);
  pinMode(pin_ECHO, INPUT);

  // // GAS
  // pinMode(pin_AUOT, OUTPUT);
}

//############ MAIN FUNCTIONS ############
void loop() {
  //ULTRASONIC
  digitalWrite(pin_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_TRIG, LOW);
  duration = pulseIn(pin_ECHO, HIGH);
  Serial.print("Distance in CM: ");
  Serial.println(duration / 59);
  ThingSpeak.setField(1,duration/59);
  delay(1000);

  // GAS SENSOR
  gas_value = analogRead(pin_AUOT);
  Serial.println(gas_value);
  ThingSpeak.setField(2,gas_value);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  //
}