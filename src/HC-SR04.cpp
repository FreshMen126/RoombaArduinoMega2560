#include "main.h"

byte ultrasonicRight = 0, ultrasonicRightAngle = 0, ultrasonicRightFront = 0, ultrasonicLeftFront = 0, ultrasonicLeftAngle = 0, ultrasonicLeft = 0;
byte ultrasonicSensor[] = {
  ultrasonicRight, 
  ultrasonicRightAngle, 
  ultrasonicRightFront, 
  ultrasonicLeftFront, 
  ultrasonicLeftAngle, 
  ultrasonicLeft
};
byte ultrasonicEchoPin[] = {
  ECHO_PIN_1, 
  ECHO_PIN_2, 
  ECHO_PIN_3, 
  ECHO_PIN_4, 
  ECHO_PIN_5, 
  ECHO_PIN_6
};
byte ultrasonicTrigPin[] = {
  TRIG_PIN_1, 
  TRIG_PIN_2, 
  TRIG_PIN_3, 
  TRIG_PIN_4, 
  TRIG_PIN_5, 
  TRIG_PIN_6
};

byte ultrasonicSensorCalculate (byte trigPin, byte echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int16_t duration, cm;
  duration = pulseIn(echoPin, HIGH);
  cm = duration / 58;
  return(cm);
}

void ultrasonicSensorLoop(){
  for (byte i = 0; i < 6; i++)
  {
    ultrasonicSensor[i] = ultrasonicSensorCalculate(ultrasonicTrigPin[i], ultrasonicEchoPin[i]);
  }
}