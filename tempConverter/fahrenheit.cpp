#include "fahrenheit.h"

Fahrenheit::Fahrenheit(QObject *parent) :
    QObject(parent)
{
    fahrenheitValue = 0;
}


void Fahrenheit::changeFahrenheitViaDial(int value)
{
    fahrenheitValue = value;
    emit fahrenheitSignalForCelsius(value);
    emit fahrenheitSignalForVisual(value);
}

void Fahrenheit::changeFahrenheitViaCels(int value)
{
    fahrenheitValue = 1.8*value+32;
    emit fahrenheitSignalForVisual(fahrenheitValue);
}
