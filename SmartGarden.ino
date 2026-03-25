#include "insGarden.h"

Actuator* actuator;

void setup() 
{
  Serial.begin(9600);
  uint8_t pinEna = 4;
  uint8_t pinOpen = 16;
  uint8_t pinClose = 17;
  actuator = new Actuator(pinEna, pinOpen, pinClose);
  actuator->activateDevice();
}

void loop() 
{
  actuator->openActuator();
  //Serial.println(actuator->getState());
  delay(3500);
  actuator->closeActuator();
  //Serial.println(actuator->getState());
  delay(3500);
}
