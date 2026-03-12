#include "insGarden.h"

void setup() 
{
  setPinOperationPump(2);
  setPinOpenValvePump(4);
}

void loop() 
{
  activationPump(2000, 3000);
}
