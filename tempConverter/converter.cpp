#include <QString>
#include "converter.h"
#include "ui_converter.h"

Converter::Converter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Converter)
{
    ui->setupUi(this);
    ui->celsiusDial->setRange(0, 100);
    ui->celsiusDial->setValue(0);
    ui->celsiusLabel->setText(QString::number(ui->celsiusDial->value()));
    ui->fahrenheitDial->setRange(32, 212);
    ui->fahrenheitDial->setValue(32);
    ui->celsiusVerticalSlider->setRange(0,100);
    ui->celsiusVerticalSlider->setValue(0);
    ui->fahrenheitVerticalSlider->setRange(32, 212);
    ui->fahrenheitVerticalSlider->setValue(32);
    //ui->fahrenheitLabel->setText(QString::number(ui->fahrenheitDial->value()));
    connect(ui->celsiusDial, SIGNAL(sliderMoved(int)), &c, SLOT(changeCelsiusViaDial(int)));
    connect(ui->celsiusVerticalSlider, SIGNAL(sliderMoved(int)), &c, SLOT(changeCelsiusViaDial(int)));
    connect(&c, SIGNAL(celsiusSignalToFahren(int)), &f, SLOT(changeFahrenheitViaCels(int)));
    connect(&c, SIGNAL(celsiusSignalToVisual(int)), this, SLOT(changeCelsius(int)));
    connect(ui->fahrenheitDial, SIGNAL(sliderMoved(int)), &f, SLOT(changeFahrenheitViaDial(int)));
    connect(ui->fahrenheitVerticalSlider, SIGNAL(sliderMoved(int)), &f, SLOT(changeFahrenheitViaDial(int)));
    connect(&f, SIGNAL(fahrenheitSignalForCelsius(int)), &c, SLOT(changeCelsiusViaFahr(int)));
    connect(&f, SIGNAL(fahrenheitSignalForVisual(int)), this, SLOT(changeFahrenheit(int)));
}

Converter::~Converter()
{
    delete ui;
}

void Converter::changeCelsius(int value)
{
    ui->celsiusLabel->setText(QString::number(value));
    ui->celsiusDial->setValue(value);
    ui->celsiusVerticalSlider->setValue(value);
}

void Converter::changeFahrenheit(int value)
{
    ui->fahrenheitLabel->setText(QString::number(value));
    ui->fahrenheitDial->setValue(value);
    ui->fahrenheitVerticalSlider->setValue(value);
}
