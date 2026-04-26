#include "insGarden.h"

Device::Device(String name, DeviceType type) 
{
  _name = name;
  _type = type;
}
Actuator::Actuator(String name, DeviceType type, uint8_t pinOpen, uint8_t pinClose, uint16_t maxLength, 
  uint16_t minLength, float lengthSecondOpen, float lengthSecondClose, float currentLength) : Device(name, type)
{
  _mask.set(FLAG_ACTIVE);
  _mask.clear(FLAG_NO_ACTIVE);
  _pinOpen = pinOpen;
  _pinClose = pinClose;
  _maxLength = maxLength;
  _minLength = minLength;
  _lengthSecondOpen = lengthSecondOpen;
  _lengthSecondClose = lengthSecondClose;
  _currentLength = currentLength;
  pinMode(_pinOpen, OUTPUT);
  pinMode(_pinClose, OUTPUT);
}
DHTSensor::DHTSensor(String name, DeviceType type, uint8_t pin, float warnTempLow, float warnTempHigh, 
  float warnHumHigh, float warnHumLow, float currentTemp, float currentHum) : Device(name, type), _HT(pin, DHT11)
{
  _warnTempLow = warnTempLow;
  _warnTempHigh = warnTempHigh;
  _warnHumHigh = warnHumHigh;
  _warnHumLow = warnHumLow;
  _currentTemp = currentTemp;
  _currentHum = currentHum;
}
DS18Sensor::DS18Sensor(String name, DeviceType type, uint8_t pin) : Device(name, type), oneWire(pin), sensors(&oneWire) {}
StepperMotorDriver::StepperMotorDriver(String name, DeviceType type, uint8_t pinENA) : Device(name, type)
{
  _pinENA = pinENA;
  pinMode(_pinENA, OUTPUT);
}
INA219Sensor::INA219Sensor(String name, DeviceType type, uint8_t pin) : Device(name, type), _ina219(pin) {}

String Device::getName()
{
  return _name;
}
DeviceType Device::getType()
{
  return _type;
}
uint16_t Device::getState()
{
  return _mask.read(FLAG_ACTIVE | FLAG_NO_ACTIVE);
}

void EventBus::subscribe(EventKey key, Device* device)
{
	_subscribers[key].push_back(device);
}
void EventBus::unsubscribe(EventKey key, Device* device)
{
	for (int i = 0; i < _subscribers[key].size(); i++)
		if (_subscribers[key][i] == device)
		{
			_subscribers[key].erase(_subscribers[key].cbegin() + i);
			return;
		}
}
void EventBus::notify(EventKey key)
{
	for (Device* device : _subscribers[key])
		device->acceptEvent(key.event);
}

void Actuator::negativeSignal()
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
void Actuator::plusSignal()
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
void Actuator::acceptEvent(EventType event)
{
	if (event == EventType::INFO)
  {
    
  }
  else if (event == EventType::WARNING_TEMP_HIGH)
  {
    
  }
  else if (event == EventType::WARNING_TEMP_LOW)
  {
    
  }
}

void DHTSensor::activateDevice()
{
  _mask.set(FLAG_ACTIVE);
  _mask.clear(FLAG_NO_ACTIVE);
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
void DHTSensor::acceptEvent(EventType event){}

void DS18Sensor::activateDevice()
{
  _mask.set(FLAG_ACTIVE);
  _mask.clear(FLAG_NO_ACTIVE);
  sensors.begin();
}
void DS18Sensor::requestTemperatures()
{
  sensors.requestTemperatures();
}
void DS18Sensor::getTempCByIndex(uint16_t index)
{
  sensors.getTempCByIndex(index);
}
void DS18Sensor::acceptEvent(EventType event){}

void StepperMotorDriver::activateDevice()
{
  _mask.set(FLAG_ACTIVE);
  _mask.clear(FLAG_NO_ACTIVE);
  digitalWrite(_pinENA, HIGH);
}
void StepperMotorDriver::acceptEvent(EventType event){}

void INA219Sensor::activateDevice()
{
  _mask.set(FLAG_ACTIVE);
  _mask.clear(FLAG_NO_ACTIVE);
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
void INA219Sensor::acceptEvent(EventType event)
{}

Device* CreatorDHTSensor::Create(JsonObject fileJson)
{
  int deviceTypeInt = fileJson["deviceType"].as<int>();
  DeviceType deviceType = static_cast<DeviceType>(deviceTypeInt);
  DHTSensor* sensor = new DHTSensor(
    fileJson["name"].as<String>(),
    deviceType,
    fileJson["pin"].as<uint8_t>(),
    fileJson["warnTempLow"].as<float>(),
    fileJson["warnTempHigh"].as<float>(),
    fileJson["warnHumLow"].as<float>(),
    fileJson["warnHumHigh"].as<float>(),
    fileJson["currentTemp"].as<float>(),
    fileJson["currentHum"].as<float>());
  return sensor;
}
Device* CreatorActuator::Create(JsonObject fileJson)
{
  int deviceTypeInt = fileJson["deviceType"].as<int>();
  DeviceType deviceType = static_cast<DeviceType>(deviceTypeInt);
  Actuator* actuator = new Actuator(
    fileJson["name"].as<String>(),
    deviceType,
    fileJson["pinOpen"].as<uint8_t>(),
    fileJson["pinClose"].as<uint8_t>(),
    fileJson["maxLength"].as<float>(),
    fileJson["minLength"].as<float>(),
    fileJson["lengthSecondOpen"].as<float>(),
    fileJson["lengthSecondClose"].as<float>(),
    fileJson["currentLength"].as<float>());
  return actuator;
}

JsonDocument loadJson(const String& fileName)
{
  JsonDocument doc;
  
  File file = SPIFFS.open(fileName, "r");
  if (!file) {
    Serial.println("Не удалось открыть файл конфигурации");
    return doc;  // Возвращаем пустой документ
  }
  
  String fileContent = file.readString();
  file.close();
  
  DeserializationError error = deserializeJson(doc, fileContent);
  if (error) {
    Serial.print("Ошибка парсинга: ");
    Serial.println(error.c_str());
  }
  
  return doc;
}