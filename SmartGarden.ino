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

  CreatorActuator creatorActuator;
  JsonArray actuators = file["actuators"].as<JsonArray>();
  for (JsonObject item : actuators)
  {
    Device* actuator = creatorActuator.Create(item);
    arrayDevices.push_back(actuator);
  }

  CreatorButtons creatorButtons;
  JsonArray buttons = file["buttons"].as<JsonArray>();
  for (JsonObject item : buttons)
  {
    Device* button = creatorButtons.Create(item);
    arrayDevices.push_back(button);
  }

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
}

void loop() 
{
  for (Device* item : arrayDevices)
  {
    if(item->getType() == DeviceType::Button)
    {
      Button* button = static_cast<Button*>(item);
      button->checkClick();
      delay(100);
    }
  }
  delay(200);
}