#include "signup.h"
#include "ui_signup.h"
#include <QDebug>

SignUp::SignUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
    qDebug() << "constructor";
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::on_pushButton_clicked()
{
    QTcpSocket signUpSocket();
//    signUpSocket.connectToHost();
}


void SignUp::slotHostFound()
{
    qDebug() << "host: " + ui->hostLineEdit->text() + " is found";
}

void SignUp::slotConnected()
{
    qDebug() << "connection established";
}

void SignUp::slotDisconnected()
{
    qDebug() << "connection lost";
}

void SignUp::slotReadData()
{
    qDebug() << "data received";
}

void SignUp::slotError()
{
    qDebug() << "have error";
}
