#include "insGarden.h"

Device* lamp;

void setup() 
{
  Serial.begin(9600);
  uint8_t pinLamp = 2;
  //lamp = new Device(pinLamp);
  lamp = new Activator(pinLamp);
}

void loop() 
{
  lamp->activateDevice();
  Serial.println(lamp->getState());
  delay(2000);
  lamp->deactivateDevice();
  Serial.println(lamp->getState());
  delay(2000);
}
