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
  //void setPinActivate(uint8_t pin);
  virtual void deactivateDevice();
  virtual void activateDevice();
  virtual uint16_t getState();
};

class Activator : public Device
{
protected:
  uint16_t FLAG_HALF_OPEN = 1 << 2;
  uint16_t FLAG_HALF_CLOSE = 1 << 3;
public:
  Activator(uint8_t pin);
  void deactivateDevice() override;
  void activateDevice() override;
  uint16_t getState() override;
};