#ifndef DIGITALPINCLASSOUTPUT_H
#define DIGITALPINCLASSOUTPUT_H
#include<Arduino.h>

class digitalPinClassOutput               //Реализация класса для работы с цифровыми выходами
{
    public:
        digitalPinClassOutput(int);       // Конструктор
        // Сеттеры
        void set_HIGH();            // Включить
        void set_LOW();             // Выключить
        void toggle_Status();       // Поменять статус
        // Геттеры
        bool get_status();
        int get_pin();       
    private:
        int pin;
        bool status;
};

#endif // DIGITALPINCLASSOUTPUT_H
