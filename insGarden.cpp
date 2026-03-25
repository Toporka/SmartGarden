#include "esp32-hal-gpio.h"
#include "insGarden.h"

Device::Device(uint8_t pin)
{
  _pinActivate = pin;
  pinMode(_pinActivate, OUTPUT);
}
Actuator::Actuator(uint8_t pinEna, uint8_t pinOpen, uint8_t pinClose) : Device(pinEna) 
{
  _pinOpen = pinOpen;
  _pinClose = pinClose;
  pinMode(_pinOpen, OUTPUT);
  pinMode(_pinClose, OUTPUT);
}

void Device::deactivateDevice()
{
  _mask.set(FLAG_NO_ACTIVE);
  _mask.clear(FLAG_ACTIVE);
  digitalWrite(_pinActivate, LOW);
}
void Device::activateDevice()
{
  _mask.set(FLAG_ACTIVE);
  _mask.clear(FLAG_NO_ACTIVE);
  digitalWrite(_pinActivate, HIGH);
}
uint16_t Device::getState()
{
  return _mask.read(FLAG_ACTIVE | FLAG_NO_ACTIVE);
}

void Actuator::closeActuator()
{
  //_mask.set(FLAG_HALF_OPEN);
  //_mask.clear(FLAG_HALF_OPEN);
  digitalWrite(_pinOpen, LOW);
  digitalWrite(_pinClose, HIGH);
}
void Actuator::openActuator()
{
  //_mask.set(FLAG_HALF_OPEN);
  //_mask.clear(FLAG_HALF_OPEN);
  digitalWrite(_pinOpen, HIGH);
  digitalWrite(_pinClose, LOW);
}
uint16_t Actuator::getState()
{
  return _mask.read(FLAG_ACTIVE | FLAG_NO_ACTIVE | FLAG_HALF_OPEN);
}
