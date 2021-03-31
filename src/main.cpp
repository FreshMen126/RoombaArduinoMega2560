#include "main.h"

void setup() 
{
  pinMode(rx_PIN, OUTPUT);
  pinMode(tx_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Roomba_init();
}

void loop() 
{
  digitalWrite(LED_PIN, HIGH);
  delay(0.5*second);
  digitalWrite(LED_PIN, LOW);
  delay(0.5*second);
}