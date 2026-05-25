#include <Arduino.h>
#include "DHT.h"
#include "RTClib.h"

RTC_DS1307 rtc;
DHT dht(0,DHT22);
int a,b;
bool c;
float h, t;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  dht.begin();
  if (! rtc.begin())
  {
    Serial.println("Ko co sensor");
    while (1);
    rtc.adjust(DateTime(2025,4,11,3,36,12));    
  }

}

//Main
void loop() {

  a = analogRead(26);
  b = analogRead(27);
  c = digitalRead(2);
  h = dht.readHumidity();
  t = dht.readTemperature();


  DateTime now = rtc.now();
  Serial.print(a*5.0/4096);
  Serial.print(' ');
  Serial.print(b*5.0/4096);
  Serial.print(' ');
  Serial.print(c);
  Serial.print(' ');
  Serial.print(t);
  Serial.print(' ');
  Serial.print(h);
  Serial.print(' ');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.println(now.second());
  delay(1000);
}