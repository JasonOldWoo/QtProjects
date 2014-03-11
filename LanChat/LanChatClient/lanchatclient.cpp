#include "lanchatclient.h"
#include "ui_lanchatclient.h"
#include <QPalette>
#include <QMessageBox>
#include <QHostAddress>

LanChatClient::LanChatClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LanChatClient)
{
    ui->setupUi(this);
//    ui->expandPushButton->setStyleSheet("background-color:#E6E6FA");
//    ui->loginPushButton->setStyleSheet("background-color:#4169e1");
    isShow = false;
    isAuthentication = true;
    ui->widget->setVisible(false);
    ui->msgTextEdit->setReadOnly(true);
    ui->msgLineEdit->setEnabled(false);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);


}

LanChatClient::~LanChatClient()
{
    delete ui;
}

void LanChatClient::on_expandPushButton_clicked()
{
    ui->widget->setVisible(isShow = !isShow);
    if (isShow)
    {
        ui->expandPushButton->setText("-");
    }
    else
    {
        ui->expandPushButton->setText("+");
    }
}

void LanChatClient::on_loginPushButton_clicked()
{
    if (ui->loginPushButton->text() == "LOGIN")
    {
        if (ui->hostLineEdit->text().isEmpty())
        {
            int ok = QMessageBox::critical(this, tr("Error"), tr("please input hostname, use default 127.0.0.1 ?"),
                                           QMessageBox::Ok | QMessageBox::Cancel);
            if (ok == QMessageBox::Cancel)
            {
                return;
            }
            ui->hostLineEdit->setText("127.0.0.1");
        }

        if (ui->portLineEdit->text().isEmpty())
        {
            int ok = QMessageBox::critical(this, tr("Error"), tr("please input port, use default 8888 ?"),
                                           QMessageBox::Ok | QMessageBox::Cancel);
            if (ok == QMessageBox::Cancel)
            {
                return;
            }
            ui->portLineEdit->setText("8888");
        }

        if (ui->usernameLineEdit->text().isEmpty())
        {
            QMessageBox::critical(this, tr("Error"), tr("username could not be empty"));
            return ;
        }

        if (ui->passwordLineEdit->text().isEmpty())
        {
            QMessageBox::critical(this, tr("Error"), tr("password could not be empty"));
        }

        socket = new QTcpSocket;

        connect(socket, SIGNAL(hostFound()), this, SLOT(slotHostFound()));
        connect(socket, SIGNAL(connected()), this, SLOT(slotSendInfo()));
        connect(this, SIGNAL(signalAuthenticated()), this, SLOT(slotConnected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadData()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError()));

        ui->loginPushButton->setEnabled(false);
        socket->connectToHost(ui->hostLineEdit->text(), ui->portLineEdit->text().toShort());
    }
    else
    {
        socket->disconnectFromHost();
    }
}

void LanChatClient::slotSendInfo()
{
    UserInfo node;
    node.setUserInfo(ui->usernameLineEdit->text(),
                     ui->passwordLineEdit->text(),
                     socket->peerAddress().toString());
    QDataStream out(socket);
    out << node;
}

void LanChatClient::on_msgLineEdit_returnPressed()
{
    if (ui->msgLineEdit->text().isEmpty())
    {
        return ;
    }
    else
    {
        //QTextStream out(socket);
        //out << ui->msgLineEdit->text();
        socket->write(ui->msgLineEdit->text().toUtf8().data(),
                      ui->msgLineEdit->text().toUtf8().length());
        ui->msgLineEdit->clear();
    }
}

void LanChatClient::slotHostFound()
{
    qDebug() << "host: " + ui->hostLineEdit->text() + " is found";
}

void LanChatClient::slotConnected()
{
    qDebug() << "connection established";
    ui->msgLineEdit->setEnabled(true);
    ui->loginPushButton->setText("LOGOUT");
    ui->signUpPushButton->setEnabled(false);
    ui->loginPushButton->setEnabled(true);
    ui->usernameLineEdit->setEnabled(false);
    ui->passwordLineEdit->setEnabled(false);
    ui->hostLineEdit->setEnabled(false);
    ui->portLineEdit->setEnabled(false);
    ui->widget->setVisible(false);
    ui->msgLineEdit->setFocus();
    isShow =  false;
    ui->expandPushButton->setText("+");
}

void LanChatClient::slotDisconnected()
{
    qDebug() << "connection lost";
    ui->msgLineEdit->setEnabled(false);
    ui->loginPushButton->setText("LOGIN");
    isAuthentication = true;
    ui->loginPushButton->setEnabled(true);
    ui->usernameLineEdit->setEnabled(true);
    ui->passwordLineEdit->setEnabled(true);
    ui->hostLineEdit->setEnabled(true);
    ui->portLineEdit->setEnabled(true);
    ui->signUpPushButton->setEnabled(true);
}

void LanChatClient::slotReadData()
{
    qDebug() << "data received";
    if (isAuthentication)
    {
        char authenRes[10] = "";
        socket->read(authenRes, 8);
        QString qAuthenRes(authenRes);
        if ("success" == qAuthenRes)
        {
            emit signalAuthenticated();
            isAuthentication = false;
        }
    }
    while (socket->bytesAvailable() > 0)
    {
        QByteArray ba;
        ba.resize(socket->bytesAvailable());
        socket->read(ba.data(), ba.size());
        ui->msgTextEdit->append(ba);
        ui->msgTextEdit->append("\n");
    }
}

void LanChatClient::slotError()
{
    qDebug() << "have error" << socket->errorString();
    ui->loginPushButton->setEnabled(true);
}

void LanChatClient::on_signUpPushButton_clicked()
{
    qDebug() << "signUp";
    signUp = new SignUp();
    signUp->show();
    this->hide();

//    signUp->close();
}
