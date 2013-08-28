#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

namespace Ui {
class Calculator;
}

class Calculator : public QWidget
{
    Q_OBJECT
    
public:
    explicit Calculator(QWidget *parent = 0);
    ~Calculator();

private:
    void displayDigit(QString);
    void displayOperator(QString);
    int isOpt(QString);
    bool rightExpr(QString&);
    
private:
    Ui::Calculator *ui;
    QString res;

public slots:
    void displayOne();
    void displayTwo();
    void displayThree();
    void displayFour();
    void displayFive();
    void displaySix();
    void displaySeven();
    void displayEight();
    void displayNine();
    void displayZero();
    void displayDot();

    void backspace();
    void clearAll();

    void add();
    void sub();
    void mul();
    void div();
    void bra();
    void ket();

    void calc();
};

#endif // CALCULATOR_H
