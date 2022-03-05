#include <Arduino.h>

#include "bt.h"

void setup() {
  /* Init UART serial ports */
  Serial.begin(115200);

  /* Init Bluetooth */
  btSetup();

  Serial.println("Started!");
}

void loop() {
  // put your main code here, to run repeatedly:
}