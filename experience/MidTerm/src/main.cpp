//############ DEFINE ############
// MQ2 GAS SENSOR 
#define GAS_PIN 34

// LED RGB
#define R_PIN 27
#define G_PIN 26
#define B_PIN 25

// LED
#define LED_PIN 13

// LDR
#define 
// RELAY
#define RELAY_PIN 15

// ULTRASONIC
#define pin_TRIG 17
#define pin_ECHO 16

//TEMPERATURE & HUMIDITY SENSOR
#define DHT_PIN 19

// 7 SEGMENT DISPLAY
#define CLK 5
#define DIO 4

// BUZZER
#define BUZZER_PIN 18

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
#include <WiFi.h>
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


}




void loop() {
    // Serial.println("TEST");
    // digitalWrite(R_PIN, HIGH);
    // digitalWrite(LED_PIN, HIGH);
    // delay(1000);
    // digitalWrite(R_PIN, LOW);
    // digitalWrite(LED_PIN, LOW);


    delay(1000);

}
