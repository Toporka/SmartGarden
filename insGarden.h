#include <Arduino.h>
#include <sys/_stdint.h>
#include <stdint.h>
#include <Adafruit_INA219.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <map>
#include <vector>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "BitFlags.h"
#include "DHT.h"

enum class DeviceType
{
	SensorTemp = 0,
	Actuator = 1,
  Driver = 2,
  SensorStrengthToque = 3
};

enum class EventType
{
	INFO = 0,
	WARNING = 1,
	WARNING_TEMP_LOW = 2,
	WARNING_TEMP_HIGH = 3,
};

struct EventKey
{
	String nameDevice;
	EventType event;

	bool operator<(const EventKey& other) const 
	{
		if (nameDevice != other.nameDevice)
			return nameDevice < other.nameDevice;
		if (event != other.event)
			return event < other.event;
		return false;
	}
};

class Device
{
private:
  String _name;
  DeviceType _type;
protected:
  // Маска
  BitFlags16 _mask;
  // Флаги состояний
  uint16_t FLAG_ACTIVE = 1 << 0;
  uint16_t FLAG_NO_ACTIVE = 1 << 1;
  Device(String name, DeviceType type);
public:
  String getName();
  DeviceType getType();
  virtual uint16_t getState();
  virtual void acceptEvent(EventType event) = 0;
};

class EventBus
{
private:
	std::map<EventKey, std::vector<Device*>> _subscribers;
public:
	void subscribe(EventKey key, Device* device);
	void unsubscribe(EventKey key, Device* device);
	void notify(EventKey key);
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
  uint32_t _timer;
  // Данные для измерения длины штопора
  uint16_t _maxLength;
  uint16_t _minLength;
  float _lengthSecondOpen;
  float _lengthSecondClose;
  float _currentLength;
public:
  Actuator(String name, DeviceType type, uint8_t pinOpen, uint8_t pinClose, uint16_t maxLength, uint16_t minLength, float lengthSecondOpen, float lengthSecondClose, float currentLength);
  void negativeSignal();
  void plusSignal();
  uint16_t getState() override;
  void acceptEvent(EventType event) override;
  float getLength();
};

class DHTSensor : public Device
{
private:
  DHT _HT;
  float _warnTempLow;
  float _warnTempHigh;
  float _warnHumLow;
  float _warnHumHigh;
  float _currentTemp;
  float _currentHum;
public:
  DHTSensor(String name, DeviceType type, uint8_t pin, float warnTempLow, float warnTempHigh, float warnHumLow, float warnHumHigh, float currentTemp, float currentHum);
  void activateDevice();
  // Получить влажность воздуха
  float getHumidity();
  // Получить температуру
  float getTemperature(bool S, bool force);
  void acceptEvent(EventType event) override;
};

class DS18Sensor : public Device
{
private:
  OneWire oneWire;
  DallasTemperature sensors;
public:
  DS18Sensor(String name, DeviceType type, uint8_t pin);
  void activateDevice();
  void requestTemperatures();
  void getTempCByIndex(uint16_t index);
  void acceptEvent(EventType event) override;
};

class StepperMotorDriver : public Device
{
  uint8_t _pinENA;
public:
  StepperMotorDriver(String name, DeviceType type, uint8_t pinENA);
  void activateDevice();
  void acceptEvent(EventType event) override;
};

class INA219Sensor : public Device
{
private:
  Adafruit_INA219 _ina219;
public:
  INA219Sensor(String name, DeviceType type, uint8_t pin);
  void activateDevice();
  float getShuntVoltageMV();
  float getBusVoltageV();
  float getCurrentMA();
  float getPoweMW();
  float getLoadVoltage();
  void acceptEvent(EventType event) override;
};

class Creator
{
public:
  virtual Device* Create(JsonObject file) = 0;
};

class CreatorDHTSensor : public Creator
{
public:
  Device* Create(JsonObject file) override;
};

class CreatorActuator : public Creator
{
public:
  Device* Create(JsonObject file) override;
};

class CreatorDS18 : public Creator
{
public:
  Device* Create(JsonObject file) override;
};

class CreatorDrivers : public Creator
{
public:
  Device* Create(JsonObject file) override;
};

class CreatorINA219 : public Creator
{
public:
  Device* Create(JsonObject file) override;
};

// Список всех устройств конфигурации
inline std::vector<Device*> arrayDevices;
// Шина событий
inline EventBus eventBus;

JsonDocument loadJson(const String& fileName);