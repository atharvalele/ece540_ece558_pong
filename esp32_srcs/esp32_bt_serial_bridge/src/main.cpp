#include <Arduino.h>
#include "network.h"

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Hello");
  setupWiFi();
  udpListener(); 
}

void loop()
{
  delay(5);
  //Send broadcast
  //Serial.println(WiFi.softAPgetStationNum());
  if(Serial1.available())
    broadcastMessage(Serial1.readString());  
}