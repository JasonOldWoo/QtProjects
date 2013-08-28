#include "calculator.h"
#include "ui_calculator.h"

Calculator::Calculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit->setAlignment(Qt::AlignRight);
    ui->lineEdit->setMaxLength(100);
    ui->lineEdit->setText("0");
    res = "0";

    connect(ui->oneButton, SIGNAL(clicked()), this, SLOT(displayOne()));
    connect(ui->twoButton, SIGNAL(clicked()), this, SLOT(displayTwo()));
    connect(ui->threeButton, SIGNAL(clicked()), this, SLOT(displayThree()));
    connect(ui->fourButton, SIGNAL(clicked()), this, SLOT(displayFour()));
    connect(ui->fiveButton, SIGNAL(clicked()), this, SLOT(displayFive()));
    connect(ui->sixButton, SIGNAL(clicked()), this, SLOT(displaySix()));
    connect(ui->sevenButton, SIGNAL(clicked()), this, SLOT(displaySeven()));
    connect(ui->eightButton, SIGNAL(clicked()), this, SLOT(displayEight()));
    connect(ui->nineButton, SIGNAL(clicked()), this, SLOT(displayNine()));
    connect(ui->zeroButton, SIGNAL(clicked()), this, SLOT(displayZero()));
    connect(ui->dotButton, SIGNAL(clicked()), this, SLOT(displayDot()));

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->subButton, SIGNAL(clicked()), this, SLOT(sub()));
    connect(ui->mulButton, SIGNAL(clicked()), this, SLOT(mul()));
    connect(ui->divButton, SIGNAL(clicked()), this, SLOT(div()));
    connect(ui->braButton, SIGNAL(clicked()), this, SLOT(bra()));
    connect(ui->ketButton, SIGNAL(clicked()), this, SLOT(ket()));
    connect(ui->calcButton, SIGNAL(clicked()), this, SLOT(calc()));

    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->backspaceButton, SIGNAL(clicked()), this, SLOT(backspace()));
}


Calculator::~Calculator()
{
    delete ui;
}

int Calculator::isOpt(QString digit)
{
    if ("+"==digit || "-"==digit || "*"==digit || "/"==digit || "("==digit || ")"==digit)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Calculator::displayDigit(QString digit)
{
    QString num = ui->lineEdit->text();
    if ("0" == num || "MATH ERROR" == num)
    {
        num = digit;
    }
    else
    {

        QString::Iterator it = num.end();
        QString q1 = *(--it);
        QString q2 = *(--it);
        if ("0" == digit)
        {
            if (num.length() >= 2)
            {
                if (q1 == "0")
                {
                    if (isOpt(q2))
                    {
                        return;
                    }
                }
            }
        }
        else
        {
            if (num.length() >= 2)
            {
                if (q1 == "0")
                {
                    if (isOpt(q2))
                    {
                        num.chop(1);
                    }
                }
            }
        }
        num.append(digit);
    }
    ui->lineEdit->setText(num);
}

void Calculator::displayOne()
{
    displayDigit("1");
}

void Calculator::displayTwo()
{
    displayDigit("2");
}

void Calculator::displayThree()
{
    displayDigit("3");
}

void Calculator::displayFour()
{
    displayDigit("4");
}

void Calculator::displayFive()
{
    displayDigit("5");
}

void Calculator::displaySix()
{
    displayDigit("6");
}

void Calculator::displaySeven()
{
    displayDigit("7");
}

void Calculator::displayEight()
{
    displayDigit("8");
}

void Calculator::displayNine()
{
    displayDigit("9");
}

void Calculator::displayZero()
{
    displayDigit("0");
}

void Calculator::displayDot()
{
    QString num = ui->lineEdit->text();
    if ("MATH ERROR" == num)
    {
        num = "0";
    }
    QString::Iterator it = num.end();
    it--;
    bool hasDot = false;
    if (isOpt(*it))
    {
        num.append("0.");
        ui->lineEdit->setText(num);
        return;
    }
    for (; it>=num.begin(); it--)
    {
        if (isOpt(*it))
        {
            break;
        }
        if ("."==*it)
        {
            hasDot = true;
            break;
        }
    }
    if (!hasDot)
    {
        num.append(".");
        ui->lineEdit->setText(num);
    }
}

void Calculator::clearAll()
{
    ui->lineEdit->setText("0");
    res = "0";
}

void Calculator::backspace()
{
    QString num = ui->lineEdit->text();
    if ("MATH ERROR" == num)
    {
        num = "0";
    }
    QString::Iterator it = num.end();
    QString last = *(--it);
    num.chop(1);
    if (num.isEmpty())
    {
        ui->lineEdit->setText("0");
        return;
    }
    ui->lineEdit->setText(num);
}

void Calculator::displayOperator(QString opt)
{
    QString num = ui->lineEdit->text();
    if ("MATH ERROR" == num)
    {
        num = "0";
    }
    QString::Iterator it = num.end();
    QString last = *(--it);

    if ("+"==last || "-"==last || "*"==last || "/"==last || "("==last)
    {
        return;
    }
    if ("."==last)
    {
        num.chop(1);
    }
    num.append(opt);
    ui->lineEdit->setText(num);
}

void Calculator::add()
{
    displayOperator("+");
}

void Calculator::sub()
{
    displayOperator("-");
}

void Calculator::mul()
{
    displayOperator("*");
}

void Calculator::div()
{
    displayOperator("/");
}

void Calculator::bra()
{
    QString num = ui->lineEdit->text();
    if ("MATH ERROR" == num)
    {
        num = "0";
    }
    QString::Iterator it = num.end();
    QString last = *(--it);
    if ("0" == num)
    {
        num.chop(1);
    }    
    if ("." == last)
    {
        num.chop(1);
    }
    num.append("(");
    ui->lineEdit->setText(num);
}

void Calculator::ket()
{
    QString num = ui->lineEdit->text();
    if ("MATH ERROR" == num)
    {
        num = "0";
        return ;
    }
    QString::Iterator it = num.end();
    QString last = *(--it);
    int bras = 0;
    int kets = 0;

    if ("(" == last || "+"==last || "-"==last || "*"==last || "/"==last)
    {
        return;
    }
    if ("." == last)
    {
        num.chop(1);
    }
    for (; it>=num.begin(); it--)
    {
        if ("(" == *it)
        {
            bras++;
        }
        if (")" == *it)
        {
            kets++;
        }
    }
    if (bras <= kets)
    {
        return;
    }
    num.append(")");
    ui->lineEdit->setText(num);
}
