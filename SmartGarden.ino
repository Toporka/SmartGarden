#include "insGarden.h"

Actuator* actuator;
float humidity;
float tempC;
float tempF;
uint8_t pinDHT = 5;
DHT HT(pinDHT, DHT11);

void setup() 
{
  Serial.begin(9600);

  uint8_t pinEna = 4;
  uint8_t pinOpen = 16;
  uint8_t pinClose = 17;
  actuator = new Actuator(pinEna, pinOpen, pinClose);
  actuator->activateDevice();

  HT.begin();
  delay(500);
}

void loop() 
{
  //Serial.println(actuator->getState());
  //actuator->openActuator();
  //delay(3500);
  //Serial.println(actuator->getState());
  //actuator->closeActuator();
  //delay(5500);

  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  tempF = HT.readTemperature(true);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature C: ");
  Serial.println(tempC);
  Serial.print("Temperature F: ");
  Serial.println(tempF);
}
