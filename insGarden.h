#include <stdint.h>
#include "esp32-hal-gpio.h"
#include <sys/_stdint.h>
#include "BitFlags.h"

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
protected:
  uint16_t FLAG_HALF_OPEN = 1 << 2;
  // Пины для открытия и закрытия актуатора
  uint8_t _pinOpen;
  uint8_t _pinClose;
public:
  Actuator(uint8_t pinEna, uint8_t pinOpen, uint8_t pinClose);
  void openActuator();
  void closeActuator();
  uint16_t getState() override;
};