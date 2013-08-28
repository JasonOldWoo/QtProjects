#ifndef CONVERTER_H
#define CONVERTER_H

#include <QWidget>
#include "fahrenheit.h"
#include "celsius.h"

namespace Ui {
class Converter;
}

class Converter : public QWidget
{
    Q_OBJECT
    
public:
    explicit Converter(QWidget *parent = 0);
    ~Converter();

public slots:
    void changeCelsius(int);
    void changeFahrenheit(int);
    
private:
    Ui::Converter *ui;
    Fahrenheit f;
    Celsius c;
};

#endif // CONVERTER_H
