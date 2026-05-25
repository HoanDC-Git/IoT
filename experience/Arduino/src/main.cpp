//############ DEFINE ############
// RELAY
#define RELAY_PIN 13

// LED RGB
#define R_PIN 8
#define G_PIN 7
#define B_PIN 6

// LED
#define LED_PIN 12

// ULTRASONIC
#define TRIG_PIN 11
#define ECHO_PIN 10

// BUZZER
#define BUZZER_PIN 9




// MQ2 GAS SENSOR 
#define GAS_PIN 34
// LDR
#define LDR_PIN A0
//TEMPERATURE & HUMIDITY SENSOR
#define DHT_PIN 19
// 7 SEGMENT DISPLAY
#define CLK 5
#define DIO 4
// OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0X3c
// PIR MOTION SENSOR
#define PIR_PIN 35



//############ LIBRARY ############
#include <Arduino.h>
#include <Wire.h>
//############ VARIABLES & CONSTANTS ############


//############


//############ SETUP ############
void setup() {
    Serial.begin(9600);

    // LED & LED RGB
    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    // RELAY
    pinMode(RELAY_PIN, OUTPUT);

    // ULTRASONIC
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // BUZZER
    pinMode(BUZZER_PIN, OUTPUT);



}




void loop() {
  // digitalWrite(R_PIN, HIGH);
  // digitalWrite(LED_PIN, HIGH);
  // delay(1000);
  // digitalWrite(R_PIN, LOW);
  // digitalWrite(LED_PIN, LOW);

  // digitalWrite(RELAY_PIN,HIGH);
  // Serial.println("HIGH");
  // delay(1000);
  // digitalWrite(RELAY_PIN,LOW);
  // Serial.println("LOW");

  // // ULTRASONIC
  // digitalWrite(TRIG_PIN, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(TRIG_PIN, LOW);
  // int duration = pulseIn(ECHO_PIN, HIGH);
  // duration = int (duration/2/29.1);
  // Serial.print("Distance in CM: ");
  // Serial.println(duration);

  // // BUZZER
  // for (int i = 100; i <= 500; ++i)
  // {
  //   // tone(BUZZER_PIN, i);
  //   // delay(150);
  //   tone(BUZZER_PIN, i, 100);
  // }
  // noTone(BUZZER_PIN);


  delay(1000);
}
