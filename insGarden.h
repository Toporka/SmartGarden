#include "BitFlags.h"
#include "EventSignal.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "DHT.h"
#include "GyverDS18.h"
#include <sys/_stdint.h>
#include <stdint.h>
#include <Adafruit_INA219.h>

class Device
{
protected:
  // Маска
  BitFlags16 _mask;
  // Флаги состояний
  uint16_t FLAG_ACTIVE = 1 << 0;
  uint16_t FLAG_NO_ACTIVE = 1 << 1;
  // Пин активации устройства
  uint8_t _pinActivate;
  Device();
public:
  Device(uint8_t pin);
  virtual void deactivateDevice();
  virtual void activateDevice();
  virtual uint16_t getState();
};

class Actuator : public Device
{
private:
  uint16_t FLAG_HALF_OPEN = 1 << 2;
  uint16_t FLAG_FULL_OPEN = 1 << 3;
  uint16_t FLAG_FULL_CLOSE = 1 << 4;
  // Пины для открытия и закрытия актуатора
  uint8_t _pinOpen;
  uint8_t _pinClose;
  // Внутренний таймер
  uint32_t _timer = 0;
  // Данные для измерения длины штопора
  const uint16_t _maxLength = 100;
  const uint16_t _minLength = 0;
  const float _lengthSecondOpen = 1.5;
  const float _lengthSecondClose = 1;
  float _currentLength = 0;
public:
  Actuator(uint8_t pinOpen, uint8_t pinClose);
  void deactivateDevice() override;
  void activateDevice() override;
  uint16_t getState() override;
  float getLength();
};

class DHTSensor : public Device
{
private:
  DHT _HT;
public:
  DHTSensor(uint8_t pin);
  void activateDevice() override;
  // Получить влажность воздуха
  float getHumidity();
  // Получить температуру
  float getTemperature(bool S, bool force);
};

class DS18Sensor : public Device
{
private:
  GyverDS18 _DS18;
public:
  DS18Sensor(uint8_t pin, bool parasite);
  uint8_t tick();
  bool setResolution(uint8_t res);
  bool setResolution(uint8_t res, uint64_t addr);
  uint8_t readResolution(uint64_t addr);
  uint8_t readPower(uint64_t addr);
  bool requestTemp();
  bool requestTemp(uint64_t addr);
  bool readTemp(uint64_t addr);
  bool readRAM(gds::RAM* ram, uint64_t addr);
  bool writeRAM(uint8_t b0, uint8_t b1, uint64_t addr);
  bool copyRAM(uint64_t addr);
  bool recallRAM(uint64_t addr);
};

class StepperMotorDriver : public Device
{
private:
  //uint8_t _pinENA;
  //uint8_t _pinIN1;
  //uint8_t _pinIN2;
  //uint8_t _pinIN3;
  //uint8_t _pinIN4;
  //static const uint8_t NO_PIN = 255;
public:
  StepperMotorDriver(uint8_t _pinENA);
  //StepperMotorDriver(uint8_t _pinENA, uint8_t _pinIN1, uint8_t _pinIN2, uint8_t _pinIN3, uint8_t _pinIN4);
  //void digitalWritePinIN(uint8_t pin, uint8_t val)
  //{
  //  if (_pinIN1 == pin) digitalWrite(_pinIN1, val);
  //  else if (_pinIN2 == pin) digitalWrite(_pinIN2, val);
  //  else if (_pinIN3 == pin) digitalWrite(_pinIN3, val);
  //  else if (_pinIN4 == pin) digitalWrite(_pinIN4, val);
  //}
};

class INA219Sensor : public Device
{
private:
  Adafruit_INA219 _ina219;
public:
  INA219Sensor(uint8_t pin);
  void activateDevice() override;
  float getShuntVoltageMV();
  float getBusVoltageV();
  float getCurrentMA();
  float getPoweMW();
  float getLoadVoltage();
};