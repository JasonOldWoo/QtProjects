#include "celsius.h"

Celsius::Celsius(QObject *parent) :
    QObject(parent)
{
    celsiusValue = 0;
}

void Celsius::changeCelsiusViaDial(int value)
{
    celsiusValue = value;
    emit celsiusSignalToFahren(value);
    emit celsiusSignalToVisual(value);
}


void Celsius::changeCelsiusViaFahr(int value)
{
    celsiusValue = (value-32.0)/1.8;
    emit celsiusSignalToVisual(celsiusValue);
}
