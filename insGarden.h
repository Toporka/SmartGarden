#include "esp32-hal-gpio.h"
//#include <Arduino.h>"

//void setPinActivationPump(uint8_t pin);
void setPinOperationPump(uint8_t pin);
void setPinOpenValvePump(uint8_t pin);
void activationPump(unsigned short timeOperation, unsigned short timeOpenValve);
//void deactivationPump();