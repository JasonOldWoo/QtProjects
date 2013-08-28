#ifndef FAHRENHEIT_H
#define FAHRENHEIT_H

#include <QObject>

class Fahrenheit : public QObject
{
    Q_OBJECT
public:
    explicit Fahrenheit(QObject *parent = 0);
    
signals:
    void fahrenheitSignalForVisual(int);
    void fahrenheitSignalForCelsius(int);

public slots:
    void changeFahrenheitViaDial(int);
    void changeFahrenheitViaCels(int);

private:
    int fahrenheitValue;
    
};

#endif // FAHRENHEIT_H
