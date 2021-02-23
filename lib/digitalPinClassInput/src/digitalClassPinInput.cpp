#include "digitalPinClassInput.h"

digitalPinClassInput::digitalPinClassInput(int pinNum)  // Конструктор, выполняется при запуске
{
    this->pin = pinNum;                                 // Сохраняется номер пина

    pinMode(pin, INPUT_PULLUP);                         // Ставится режим чтения

    this->lastState = !bool(digitalRead(pin));          // Запись текущего состояния
}

bool digitalPinClassInput::read_data()                  // Чтение данных
{
    if(digitalRead(pin) == HIGH)                               
        lastState = true;                               

    else
        lastState = false;   
     
    return !lastState;                                  // Возврат прочтённого состояния, инверсированного для нормальной работы режима input_pullup
}

bool digitalPinClassInput::is_changed()                 // Изменилось ли состояние        
{
    bool cur_state = !(bool(digitalRead(pin)));         // Чтение данных о состоянии пина

    if(lastState != cur_state)                          // Если состояние изменилось           
    {
        lastState = cur_state;                          // Сохраняется состояние
        return true;                                    // Возвращается true
    }
    else                                                // В ином случае
    {
        return false;                                   // Возвращается false
    }
    
}

int digitalPinClassInput::get_pin()
{
    return pin;
}