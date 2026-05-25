#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define RESET -1
#define ADDRESS 0x3c
Adafruit_SSD1306 display (WIDTH, HEIGHT, &Wire, RESET);


int x = 0;
void setup() {
  pinMode(19, OUTPUT);
  pinMode(15,OUTPUT);
  digitalWrite(15, LOW);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Hello World !!!!");
  display.display();

}

void loop() {
  delay(1000);
  x += 1;
  if (x > 200) x = 0;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Hello World !!!!");
  display.setTextSize(3);
  display.setCursor(64,32);
  display.println(x);
  display.display();
  
}
