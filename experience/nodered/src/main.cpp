//############ DEFINE ############
// LED RGB
#define R_PIN 13
#define G_PIN 12
#define B_PIN 14

//TEMPERATURE & HUMIDITY SENSOR
#define DHT_pin 5 

//############ LIBRARY ############
#include <Arduino.h>
#include <WiFi.h>
#include "DHT.h"
#include <PubSubClient.h>

//############ VARIABLES & CONSTANTS ############
float temperature, humidity;

const char *ssid = "Kfloor";
const char *password = "Chaongaymoi";

const char *mqtt_broker = "broker.emqx.io";
const char *mqtt_user = "emqx";
const char *mqtt_pass = "public";
const int mqtt_port = 1883;
String clientId = "esp32-" + String(WiFi.macAddress());


//############
// DHT dht(DHT_pin,DHT22);
WiFiClient espClient;
PubSubClient client(espClient);


unsigned long lastSend = 0; // Biến dùng millis
const unsigned long interval = 5000;

void callback(char *topic, byte *payload, unsigned int length);
void reconnect(); 

//############ SETUP ############
void setup() {
  Serial.begin(9600);

  // LED RGB
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  digitalWrite(R_PIN, LOW);
  digitalWrite(G_PIN, LOW);
  digitalWrite(B_PIN, LOW);

  // // TEMPERATURE & HUMIDITY SENSOR
  // dht.begin();

 //
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

void callback(char *topic, byte *payload, unsigned int length) 
{
  String msg;
  for (unsigned int i = 0; i < length; i++) 
    msg += (char)payload[i];
  Serial.print(msg);

  bool on = (msg.equalsIgnoreCase("true"));
  if (strcmp(topic, "LED/red") == 0) 
    digitalWrite(R_PIN, on);
  else if (strcmp(topic, "LED/green") == 0) 
    digitalWrite(G_PIN, on);
  else if (strcmp(topic, "LED/blue") == 0) 
    digitalWrite(B_PIN, on);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Loading...");

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      Serial.println("Connected");

      client.subscribe("LED/red");
      client.subscribe("LED/green");
      client.subscribe("LED/blue");
    }
    else
    {
      delay(5000);
    }
  }
  
}

void loop() {
  if (!client.connected()) reconnect();

  client.loop();
  unsigned long now = millis();
  if (now - lastSend > interval)
  {
    lastSend = now;
    humidity = 50; //dht.readHumidity();
    temperature = 50; //dht.readTemperature();

    if (!isnan(temperature)) 
    client.publish("DHT/temp", String(temperature, 2).c_str());
    if (!isnan(humidity)) 
    client.publish("DHT/humi", String(humidity, 2).c_str());
  }
  Serial.print(humidity);
  Serial.print(" ");
  Serial.println(temperature);
}