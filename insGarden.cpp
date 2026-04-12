#include "insGarden.h"

Device::Device() {}
Device::Device(uint8_t pin)
{
  _pinActivate = pin;
  pinMode(_pinActivate, OUTPUT);
}
Actuator::Actuator(uint8_t pinOpen, uint8_t pinClose)
{
  _pinOpen = pinOpen;
  _pinClose = pinClose;
  pinMode(_pinOpen, OUTPUT);
  pinMode(_pinClose, OUTPUT);
}
DHTSensor::DHTSensor(uint8_t pin) : Device(pin), _HT(pin, DHT11) {}
DS18Sensor::DS18Sensor(uint8_t pin, bool parasite = true) : Device(pin), _DS18(pin, parasite) {}
StepperMotorDriver::StepperMotorDriver(uint8_t _pinENA) : Device(_pinENA) {}
//StepperMotorDriver::StepperMotorDriver(uint8_t _pinENA, uint8_t _pinIN1 = NO_PIN, uint8_t _pinIN2 = NO_PIN, uint8_t _pinIN3 = NO_PIN, uint8_t _pinIN4 = NO_PIN) : Device(_pinENA)
//{
//  if (_pinIN1 != NO_PIN) pinMode(_pinIN1, OUTPUT);
//  if (_pinIN2 != NO_PIN) pinMode(_pinIN2, OUTPUT);
//  if (_pinIN3 != NO_PIN) pinMode(_pinIN3, OUTPUT);
//  if (_pinIN4 != NO_PIN) pinMode(_pinIN4, OUTPUT);
//}
INA219Sensor::INA219Sensor(uint8_t pin) : Device(pin), _ina219(pin) {}

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

void Actuator::deactivateDevice()
{
  uint16_t time = (millis() - _timer) / 1000;
  _currentLength -= time * _lengthSecondClose;
  _timer = millis();
  if (_currentLength <= _minLength)
  {
    _currentLength = _minLength;
    _mask.clear(FLAG_HALF_OPEN);
    _mask.set(FLAG_FULL_CLOSE);
  }
  else
  {
    _mask.clear(FLAG_FULL_OPEN);
    _mask.set(FLAG_HALF_OPEN);
  }
  digitalWrite(_pinOpen, LOW);
  digitalWrite(_pinClose, HIGH);
}
void Actuator::activateDevice()
{
  uint16_t time = (millis() - _timer) / 1000;
  _currentLength += time * _lengthSecondOpen;
  _timer = millis();
  if (_currentLength >= _maxLength)
  {
    _currentLength = _maxLength;
    _mask.clear(FLAG_HALF_OPEN);
    _mask.set(FLAG_FULL_OPEN);
  }
  else
  {
    _mask.clear(FLAG_FULL_CLOSE);
    _mask.set(FLAG_HALF_OPEN);
  }
  digitalWrite(_pinOpen, HIGH);
  digitalWrite(_pinClose, LOW);
}
uint16_t Actuator::getState()
{
  return _mask.read(FLAG_ACTIVE | FLAG_NO_ACTIVE | FLAG_HALF_OPEN | FLAG_FULL_OPEN | FLAG_FULL_CLOSE);
}
float Actuator::getLength()
{
  return _currentLength;
}

void DHTSensor::activateDevice()
{
  _HT.begin();
}
float DHTSensor::getHumidity()
{
  return _HT.readHumidity();
}
float DHTSensor::getTemperature(bool S, bool force)
{
  return _HT.readTemperature(S, force);
}

uint8_t DS18Sensor::tick()
{
  return _DS18.tick();
}
bool DS18Sensor::setResolution(uint8_t res)
{
  return _DS18.setResolution(res);
}
bool DS18Sensor::setResolution(uint8_t res, uint64_t addr)
{
  return _DS18.setResolution(res, addr);
}
uint8_t DS18Sensor::readResolution(uint64_t addr)
{
  return _DS18.readResolution(addr);
}
uint8_t DS18Sensor::readPower(uint64_t addr)
{
  return _DS18.readPower(addr);
}
bool DS18Sensor::requestTemp()
{
  return _DS18.requestTemp();
}
bool DS18Sensor::requestTemp(uint64_t addr)
{
  return _DS18.requestTemp(addr);
}
bool DS18Sensor::readTemp(uint64_t addr)
{
  return _DS18.readTemp(addr);
}
bool DS18Sensor::readRAM(gds::RAM* ram, uint64_t addr)
{
  return _DS18.readRAM(ram, addr);
}
bool DS18Sensor::writeRAM(uint8_t b0, uint8_t b1, uint64_t addr)
{
  return _DS18.writeRAM(b0, b1, addr);
}
bool DS18Sensor::copyRAM(uint64_t addr)
{
  return _DS18.copyRAM(addr);
}
bool DS18Sensor::recallRAM(uint64_t addr)
{
  return _DS18.recallRAM(addr);
}

void INA219Sensor::activateDevice()
{
  _ina219.begin();
}
float INA219Sensor::getShuntVoltageMV()
{
  return _ina219.getShuntVoltage_mV();
}
float INA219Sensor::getBusVoltageV()
{
  return _ina219.getBusVoltage_V();
}
float INA219Sensor::getCurrentMA()
{
  return _ina219.getCurrent_mA();
}
float INA219Sensor::getPoweMW()
{
  return _ina219.getPower_mW();
}
float INA219Sensor::getLoadVoltage()
{
  return getBusVoltageV() + (getShuntVoltageMV() / 1000);
}