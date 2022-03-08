#include <Arduino.h>
#include "network.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello");
  setupWiFi();
  udpListener(); 
}

void loop()
{
    delay(1000);
    //Send broadcast
    //Serial.println(WiFi.softAPgetStationNum());
}