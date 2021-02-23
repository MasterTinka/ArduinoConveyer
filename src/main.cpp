#include <Arduino.h>
#include <digitalPinClassOutput.h>
#include <digitalPinClassInput.h>
#include <GyverTimer.h>

GTimer Timer15Sec(MS), Timer30Sec(MS);
digitalPinClassInput accidentOnConveyer(22), startConveyers(23), stopConveyers(25);
digitalPinClassOutput conveyer170(51), conveyer36(52), bunker(53), buzzer(49), accident(50);

digitalPinClassOutput* conveyers[3] = {&conveyer170, &conveyer36, &bunker};

void setup() {
  conveyer170.set_HIGH();
  conveyer36.set_HIGH();
  bunker.set_HIGH();
  buzzer.set_HIGH();
  accident.set_HIGH();

  Timer30Sec.setInterval(30000);
  Timer30Sec.stop();
  Timer15Sec.setInterval(15000);
  Timer15Sec.stop();
}


void resetConveyers()
{
  for(int8_t i = 0; i < 3; i++)
  {
    conveyers[i]->set_HIGH();
  }
  buzzer.set_HIGH();
}

void accidentReset()
{
  accident.set_LOW();
  resetConveyers();
}

bool buzzerAction()
{
  Timer30Sec.start();
  int8_t amount = 0;
  while(amount < 3)
  {
    buzzer.toggle_Status();
    while(!Timer30Sec.isReady())
    {
      delay(1);
      if(accidentOnConveyer.read_data() == false)
      {
        accidentReset();
        return false;
      }
    }
    amount++;    
  }
  buzzer.set_HIGH();
  Timer30Sec.stop();
  return true;
}

bool startConveyersFunction()
{
  Timer15Sec.start();
  for(int i = 0; i < 3; i++)
  {
    conveyers[i]->set_LOW();
    while(!Timer15Sec.isReady() && i != 2)
    {
      delay(1);
      if(accidentOnConveyer.read_data() == false)
      {
        accidentReset();
        return false;
      }
    }
  }
  Timer15Sec.stop();
  return true;
}

void loop() {
  if(accidentOnConveyer.read_data() && startConveyers.read_data())
  {
    if(buzzerAction() == false)
    {
      return;
    }
    if(startConveyersFunction() == false)
    {
      return;
    }
    while (true)
    {
      if(stopConveyers.read_data())
      {
        resetConveyers();
        break;
      }
      else if(accidentOnConveyer.read_data() == false)
      {
        accidentReset();
        break;
      }
    }
  }
  else if(accidentOnConveyer.read_data() && accident.get_status() == false)
  {
    accident.set_HIGH();
  }
}