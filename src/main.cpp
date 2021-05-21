#include "main.h"
#include "stdio.h"

void setup() 
{
  pinMode(rx_PIN, OUTPUT);
  pinMode(tx_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Roomba_init();
}

void loop() 
{
  Roomba_Loop();
}