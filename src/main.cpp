#include <Arduino.h>
#include <digitalPinClassOutput.h>
#include <digitalPinClassInput.h>
#include <GyverTimer.h>

GTimer Timer15Sec(MS), Timer30Sec(MS);
digitalPinClassInput accidentOnConveyer(24), accidentGone(22), startConveyers(23), stopConveyers(25);
digitalPinClassOutput conveyer170(51), conveyer36(52), bunker(53), buzzer(49), accident(50);

digitalPinClassOutput* conveyers[3] = {&conveyer170, &conveyer36, &bunker};

bool accidentState = false;

String message;

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

  Serial.begin(115200);
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
  Serial.print("BUZZER STARTED");
  Timer30Sec.start();
  int8_t amount = 0;
  while(amount < 3)
  {
    buzzer.toggle_Status();
    while(!Timer30Sec.isReady())
    {
      delay(1);
      if(Serial.available())
      {
        while (Serial.available())
        {
          char recieved_byte = Serial.read();
          message += recieved_byte;
        }
      }
      if(accidentOnConveyer.read_data() || message == "ACCIDENT")
      {
        accidentState = true;
        Serial.print("BUZZER GONE");
        Serial.print("CONVEYERS GONE");
        Serial.print("ACCIDENT STARTED");
        accidentReset();
        return false;
      }
      else if(stopConveyers.read_data() || message == "STOP")
      {
        Serial.print("BUZZER GONE");
        Serial.print("CONVEYERS GONE");
        resetConveyers();
        return false;
      }
    }
    amount++;    
  }
  buzzer.set_HIGH();
  Timer30Sec.stop();
  Serial.print("BUZZER GONE");
  return true;
}

bool startConveyersFunction()
{
  Serial.print("CONVEYERS_START STARTED");
  Timer15Sec.start();
  for(int i = 0; i < 3; i++)
  {
    conveyers[i]->set_LOW();
    while(!Timer15Sec.isReady() && i != 2)
    {
      delay(1);
      if(Serial.available())
      {
        while (Serial.available())
        {
          char recieved_byte = Serial.read();
          message += recieved_byte;
        }
      }
      if(accidentOnConveyer.read_data() || message == "ACCIDENT")
      {
        accidentState = true;
        Serial.print("CONVEYERS_START GONE");        
        Serial.print("CONVEYERS GONE");
        Serial.print("ACCIDENT STARTED");        
        accidentReset();
        return false;
      }
      else if(stopConveyers.read_data() || message == "STOP")
      {
        Serial.print("CONVEYERS_START GONE");
        Serial.print("CONVEYERS GONE");
        resetConveyers();
        return false;
      }
    }
  }
  Serial.print("CONVEYERS_START GONE");
  Timer15Sec.stop();
  return true;
}

void loop() {

  while (Serial.available())
  {
    char recieved_byte = Serial.read();
    message += recieved_byte;
  }

  if((accidentState == false) && ( startConveyers.read_data() || (message == "START") ))
  {
    message = "";
    Serial.print("CONVEYERS STARTED");
    if(buzzerAction() == false)
    {
      message = "";
      return;
    }
    if(startConveyersFunction() == false)
    {
      message = "";
      return;
    }
    while (true)
    {
      if(Serial.available())
      {
        while (Serial.available())
        {
          char recieved_byte = Serial.read();
          message += recieved_byte;
        }
      }
      if(stopConveyers.read_data() || message == "STOP")
      {
        message = "";
        Serial.print("CONVEYERS GONE");
        resetConveyers();
        break;
      }
      else if(accidentOnConveyer.read_data() || message == "ACCIDENT") 
      {
        Serial.print("ACCIDENT STARTED");
        accidentState = true;
        message = "";
        accidentReset();
        break;
      }
    }
  }
  else if((accidentGone.read_data() || message == "STOP_ACCIDENT") && accidentState)
  {
    Serial.print("ACCIDENT GONE");
    accidentState = false;
    message = "";
    accident.set_HIGH();
  }
}