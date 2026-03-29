#include "BitFlags.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "DHT.h"
#include <sys/_stdint.h>
#include <stdint.h>

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
  const uint16_t _maxLength = 200;
  const uint16_t _minLength = 0;
  const float _lengthSecondOpen = 1.5;
  const float _lengthSecondClose = 1;
  float _currentLength = 0;
public:
  Actuator(uint8_t pinEna, uint8_t pinOpen, uint8_t pinClose);
  void openActuator();
  void closeActuator();
  uint16_t getState() override;
};