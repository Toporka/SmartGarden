# Обзор
Система управления умным садом на базе ESP32. Поддерживает различные датчики и исполнительные устройства с системой событий.
# Поддерживаемые устройства
DHTSensor: Датчик температуры/влажности DHT11
DS18Sensor: Датчик температуры DS18B20 (1-Wire)
INA219Sensor: Датчик тока/напряжения INA219
Actuator: Линейный актуатор (открытие/закрытие)
StepperMotorDriver: Драйвер шагового двигателя
Button: Кнопка (события нажатия)
# Конфигурация
**Файл конфигурации** - Создайте файл /fileDevices.json в SPIFFS следующей структуры:
{
  "actuators": [...],
  "buttons": [...],
  "routes": [...]
}
**Конфигурация актуатора:**
{
  "name": "valve1",
  "deviceType": 1,
  "pinOpen": 12,
  "pinClose": 13,
  "maxLength": 100,
  "minLength": 0,
  "lengthSecondOpen": 1.0,
  "lengthSecondClose": 1.0,
  "currentLength": 50.0
}
**Конфигурация датчика DHT:**
{
  "name": "dht1",
  "deviceType": 0,
  "pin": 4,
  "warnTempLow": 10.0,
  "warnTempHigh": 35.0,
  "warnHumLow": 30.0,
  "warnHumHigh": 80.0,
  "currentTemp": 22.5,
  "currentHum": 55.0
}
**Конфигурация датчика DS18:**
{
  "name": "DS18 #1",
  "deviceType": 0,
  "pin": 18
}
**Конфигурация драйвера:**
{
  "name": "Driver #1",
  "deviceType": 2,
  "pin": 19
}
**Конфигурация датчика силы тока:**
{
  "name": "INA219 #1",
  "deviceType": 3,
  "pin": 22
}
**Конфигурация кнопки:**
{
  "name": "btn1",
  "deviceType": 4,
  "pin": 14
}
**Настройка маршрутов событий:**
{
  "namePublisher": "btn1",
  "nameSubscribers": ["valve1"],
  "events": [4]
}
#Типы событий:
0 - INFO
1 - WARNING
2 - WARNING_TEMP_LOW
3 - WARNING_TEMP_HIGH
4 - BUTTON_CLICK
5 - BUTTON_NO_CLICK
#Библиотеки
Adafruit_INA219.h
OneWire.h
DallasTemperature.h
ArduinoJson.h
SPIFFS.h
BitFlags.h
DHT.h