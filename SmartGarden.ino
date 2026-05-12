#include "insGarden.h"

void setup() 
{
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("Ошибка монтирования SPIFFS");
    return;
  }

  const String filePath = "/fileDevices.json";
  JsonDocument file;
  try
  {
    file = loadJson(filePath);
  }
  catch (const std::exception& ex)
  {
    Serial.println(ex.what());
    return;
  }

  CreatorDHTSensor creatorDHTSensor;
  JsonArray sensors = file["DHTsensors"].as<JsonArray>();
  for (JsonObject item : sensors)
  {
    Device* sensor = creatorDHTSensor.Create(item);
    arrayDevices.push_back(sensor);
  }

  CreatorActuator creatorActuator;
  JsonArray actuators = file["actuators"].as<JsonArray>();
  for (JsonObject item : actuators)
  {
    Device* actuator = creatorActuator.Create(item);
    arrayDevices.push_back(actuator);
  }

  EventBus eventBus;
  JsonArray routes = file["routes"].as<JsonArray>();
  for (JsonObject item : routes)
  {
    JsonArray nameSubscribers = item["nameSubscribers"].as<JsonArray>();
    for (String name : nameSubscribers)
    {
      for (Device* device : arrayDevices)
      {
        if (name == device->getName())
        {
          JsonArray events = item["events"].as<JsonArray>();
          for (int number : events)
          {
            eventBus.subscribe({ item["namePublisher"].as<String>(), static_cast<EventType>(number) }, device);
          }
        }
      }
    }
  }

  EventKey event{ "Sensor #1", EventType::INFO };
  eventBus.notify(event);
}

void loop() 
{
  delay(5000);
  for(Device* device : arrayDevices)
  {
    Serial.println(device->getName().c_str());
  }
}