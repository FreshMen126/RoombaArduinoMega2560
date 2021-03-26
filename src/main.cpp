#include "main.h"

void setup() {
  Roomba_init();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(second);
  digitalWrite(LED_PIN, LOW);
  delay(second);
}