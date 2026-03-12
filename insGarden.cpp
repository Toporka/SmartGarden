#include "insGarden.h"

//uint8_t pinActivationPump;
uint8_t _pinOperationPump;
uint8_t _pinOpenValvePump;

/*void setPinActivationPump(uint8_t pin)
{
  pinActivationPump = pin;
  pinMode(pinActivationPump, OUTPUT);
}*/
void setPinOperationPump(uint8_t pin)
{
  _pinOperationPump = pin;
  pinMode(_pinOperationPump, OUTPUT);
}
void setPinOpenValvePump(uint8_t pin)
{
  _pinOpenValvePump = pin;
  pinMode(_pinOpenValvePump, OUTPUT);
}
void activationPump(unsigned short operationTimeMs, unsigned short openValveTimeMs)
{
  // Проверка: null значения
  if (_pinOperationPump == NULL || _pinOpenValvePump == NULL)
    throw "The pin has a null value!";
  // Проверка: Насос заполнен водой?
  // ...
  // Закрытие клапана, начало работы насоса
  digitalWrite(_pinOpenValvePump, LOW);
  digitalWrite(_pinOperationPump, HIGH);
  delay(operationTimeMs);
  // Прекращение работы насоса, открытие клапана
  digitalWrite(_pinOperationPump, LOW);
  digitalWrite(_pinOpenValvePump, HIGH);
  delay(openValveTimeMs);
}
/*void deactivationPump()
{
  digitalWrite(_pinOperationPump, LOW);
  digitalWrite(_pinOpenValvePump, LOW);
}*/