#include "signup.h"
#include <QDebug>

SignUp::SignUp(QWidget *parent): QDialog(parent)
{
    qDebug() << "constructor";
    this->setGeometry(400, 200, 400, 470);
    pushButton = new QPushButton(this);
    pushButton->setGeometry(150, 400, 100, 60);
    connect(pushButton, SIGNAL(clicked()), SLOT(on_pushButton_clicked()));
}

SignUp::~SignUp()
{
}

void SignUp::on_pushButton_clicked()
{
    QTcpSocket signUpSocket();
//    signUpSocket.connectToHost();
    emit signalPushButtonClicked();
}


void SignUp::slotHostFound()
{
    qDebug() << "host is found";
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
