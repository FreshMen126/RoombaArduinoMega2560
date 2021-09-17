#include "main.h"
#include "stdio.h"
#include "deps/GyverTimers.h"

void setup()
{
  pinMode(RX_PIN, OUTPUT);
  pinMode(TX_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Timer1.setPeriod(5000);
  Timer1.enableISR();
  /*
  cli();                 // отключить глобальные прерывания
  TCCR0A = 0;            // установить TCCR1A регистр в 0
  TCCR0B = 0;            // установить TCCR1B регистр в 0
  TIMSK0 = (1 << TOIE0); // включить прерывание Timer1 overflow
  TCCR0B |= (1 << CS02) | (1 << CS01) | (1 << CS00); // Установить  биты ()
  sei();                   // включить глобальные прерывания
  */
  roomba_Init();
}

void loop()
{
  roomba_Loop();
}

ISR(TIMER1_A) 
{
  roomba_SensorsLoop();
}