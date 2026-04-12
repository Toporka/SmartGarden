#include "insGarden.h"

Actuator* actuator;

void setup() 
{
  Serial.begin(9600);

  uint8_t pinOpen = 16;
  uint8_t pinClose = 17;
  actuator = new Actuator(pinOpen, pinClose);
  actuator->activateDevice();
}

void loop() 
{
  Serial.println(actuator->getState());
  actuator->activateDevice();
  delay(3500);
  Serial.println(actuator->getState());
  actuator->deactivateDevice();
  delay(5500);
}
