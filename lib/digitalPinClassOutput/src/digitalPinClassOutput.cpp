#include "digitalPinClassOutput.h"

digitalPinClassOutput::digitalPinClassOutput(int pinNum)
{
    this->pin = pinNum;
    this->status = false;
    pinMode(pin, OUTPUT);
}

void digitalPinClassOutput::set_LOW()
{
    if (this->status == true)
    {
        this->status = false;
        digitalWrite(pin, LOW);
    }
}

void digitalPinClassOutput::set_HIGH()
{
    if (this->status == false)
    {
        this->status = true;
        digitalWrite(pin, HIGH);
    }
}

void digitalPinClassOutput::toggle_Status()
{
    if (this->status == true)
    {
        this->status = false;
        digitalWrite(pin, LOW);
    }
    else if (this->status == false)
    {
        this->status = true;
        digitalWrite(pin, HIGH);
    }
}

bool digitalPinClassOutput::get_status()
{
    return this->status;
}

int digitalPinClassOutput::get_pin()
{
    return this->pin;
}