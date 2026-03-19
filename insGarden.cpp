#include "insGarden.h"

Device::Device(uint8_t pin)
{
  _pinActivate = pin;
  pinMode(_pinActivate, OUTPUT);
}
Activator::Activator(uint8_t pin) : Device(pin) {}

/*void Device::setPinActivate(uint8_t pin)
{
  _pinActivate = pin;
  pinMode(_pinActivate, OUTPUT);
}*/
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

void Activator::deactivateDevice()
{
  _mask.set(FLAG_HALF_CLOSE);
  _mask.clear(FLAG_HALF_OPEN);
  digitalWrite(_pinActivate, LOW);
}
void Activator::activateDevice()
{
  _mask.set(FLAG_HALF_OPEN);
  _mask.clear(FLAG_HALF_CLOSE);
  digitalWrite(_pinActivate, HIGH);
}
uint16_t Activator::getState()
{
  return _mask.read(FLAG_ACTIVE | FLAG_NO_ACTIVE | FLAG_HALF_CLOSE | FLAG_HALF_OPEN);
}
