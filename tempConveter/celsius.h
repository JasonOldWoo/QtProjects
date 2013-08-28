#ifndef CESIUS_H
#define CESIUS_H

#include <QObject>

class Celsius : public QObject
{
    Q_OBJECT
public:
    explicit Celsius(QObject *parent = 0);
    
signals:
    void celsiusSignalToVisual(int);
    void celsiusSignalToFahren(int);
    
public slots:
    void changeCelsiusViaDial(int);
    void changeCelsiusViaFahr(int);

private:
    int celsiusValue;
    
};

#endif // CESIUS_H
