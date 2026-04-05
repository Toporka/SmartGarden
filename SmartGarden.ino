#include "insGarden.h"

class SensorTemp
{
private:
  int temp = 12;
public:
  Signal<int> signalUpdateTemp;
  void updateTemp()
  {
    temp += 1;
    signalUpdateTemp.emit(temp);
  }
};

class ManagerTemp1
{
public:
  void printTemp(int temp)
  {
    Serial.print("M1: ");
    Serial.println(temp);
  }
};

class ManagerTemp2
{
public:
  void printTemp(int temp)
  {
    Serial.print("M2: ");
    Serial.println(temp);
  }
};

void printTemp(int temp)
{
  Serial.print("Function: ");
  Serial.println(temp);
}

//Actuator* actuator;
SensorTemp sensor;
ManagerTemp1 manager1;
ManagerTemp2 manager2;

void setup() 
{
  Serial.begin(9600);

  sensor.signalUpdateTemp.connect(&manager1, &ManagerTemp1::printTemp);
  sensor.signalUpdateTemp.connect(&manager2, &ManagerTemp2::printTemp);
  sensor.signalUpdateTemp.connect(printTemp);
  //uint8_t pinEna = 4;
  //uint8_t pinOpen = 16;
  //uint8_t pinClose = 17;
  //actuator = new Actuator(pinEna, pinOpen, pinClose);
  //actuator->activateDevice();
}

void loop() 
{
  sensor.updateTemp();
  delay(2000);
  //Serial.println(actuator->getState());
  //actuator->openActuator();
  //delay(3500);
  //Serial.println(actuator->getState());
  //actuator->closeActuator();
  //delay(5500);
}
