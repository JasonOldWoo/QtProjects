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
    isShow = true;
    isAuthentication = true;
    ui->expandPushButton->setText("-");
    ui->widget->setVisible(true);
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
            return ;
        }

        socket = new QTcpSocket;

        connect(socket, SIGNAL(hostFound()), this, SLOT(slotHostFound()));
        connect(socket, SIGNAL(connected()), this, SLOT(slotSendInfo()));
        connect(this, SIGNAL(signalAuthenticated()), this, SLOT(slotConnected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadData()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError()));

        socket->connectToHost(ui->hostLineEdit->text(), ui->portLineEdit->text().toShort());
        if (!socket->waitForConnected())
        {
            QMessageBox::warning(this, tr("Error"), tr("Can not connect to server, please check your network"));
        }
    }
    else
    {
        socket->disconnectFromHost();
        ui->loginPushButton->setText(tr("LOGIN"));
        ui->hostLineEdit->setReadOnly(false);
        ui->portLineEdit->setReadOnly(false);
        ui->usernameLineEdit->setReadOnly(false);
        ui->passwordLineEdit->setReadOnly(false);
    }
}

void LanChatClient::slotSendInfo()
{
//    UserInfo node;
    quint16 shPdu = LCDB_UserLogin_Rep_FromCli;
    quint32 dwUserType = 10;
//    node.setUserInfo(ui->usernameLineEdit->text(),
//                     ui->passwordLineEdit->text(),
//                     socket->peerAddress().toString());
    QByteArray ba;
    QDataStream out(&ba, QIODevice::ReadWrite);
    out << shPdu;
    out << ui->usernameLineEdit->text();
    out << ui->passwordLineEdit->text();
    out << dwUserType;
    out << socket->localAddress().toString();
    socket->write(ba.data(), (qint64) ba.length());
//    out << node;
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
    ui->usernameLineEdit->setReadOnly(false);
    ui->passwordLineEdit->setReadOnly(false);
    ui->hostLineEdit->setReadOnly(false);
    ui->portLineEdit->setReadOnly(false);
    isShow = true;
    ui->expandPushButton->setText("-");
    ui->widget->setVisible(true);
}

void LanChatClient::slotReadData()
{
    QByteArray ba = socket->readAll();
    qDebug() << "void LanChatClient::slotReadData() - inlen=[" << ba.size() << "]";
    if (ba.size())
    {
        QDataStream d(&ba, QIODevice::ReadOnly);
        quint16 shPdu;
        d >> shPdu;
        qDebug() << "void LanChatClient::slotReadData() - PDU=[" << shPdu << "]";
        switch (shPdu)
        {
        case LCDB_UserLogin_Rsp_ToCli:
            dillAuthInfo(ba.data()+sizeof(shPdu), ba.length()-sizeof(shPdu));
            break;
        case LCDB_KickUser_Req_ToCli:
            socket->disconnectFromHost();
            QMessageBox::warning(this, tr("Error"), tr("This account has been accessed in another place!"));
            break;
        case LCDB_GetFriendList_Rsp_ToCli:
            dillFriendList(ba.data()+sizeof(shPdu), ba.length()-sizeof(shPdu));
            break;
        }
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
}

void LanChatClient::dillAuthInfo(const char *inbuf, uint len)
{
    QByteArray inBa(inbuf, len);
    QDataStream inD(&inBa, QIODevice::ReadOnly);
    qint16 shRet;
    inD >> shRet;
    if (LCDB_ERR_SUCCESS == shRet)
    {
        ui->loginPushButton->setText(tr("LOGOUT"));
        ui->hostLineEdit->setReadOnly(true);
        ui->passwordLineEdit->setReadOnly(true);
        ui->portLineEdit->setReadOnly(true);
        ui->usernameLineEdit->setReadOnly(true);
        ui->msgLineEdit->setEnabled(true);
        ui->msgLineEdit->setReadOnly(false);
        ui->msgTextEdit->setEnabled(true);
        ui->msgTextEdit->setReadOnly(true);
        ui->expandPushButton->setText("+");
        isShow = false;
        ui->widget->setVisible(false);
        inD >> m_userIndex;
//        QByteArray outBa;
//        QDataStream outD(&outBa, QIODevice::ReadWrite);
//        outD << LCDB_GetFriendList_Rep_FromCli;
//        outD << m_userIndex;
//        socket->write(outBa.data(), outBa.length());
    }
    else if (LCDB_ERR_DBDATA == shRet)
    {
        QMessageBox::warning(this, tr("Error"), tr("Server error!"));
        socket->disconnectFromHost();
    }
    else if (LCDB_ERR_USER_Online == shRet)
    {
        usleep(500);
        slotSendInfo();
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Username or password wrong!"));
        socket->disconnectFromHost();
    }
}

void LanChatClient::dillFriendList(const char *inbuf, uint len)
{
    QByteArray inBa(inbuf, len);
    QDataStream inD(&inBa, QIODevice::ReadOnly);
    qint16 shRet;
    inD >> shRet;
    if (LCDB_ERR_SUCCESS == shRet)
    {
        quint32 dwUserNum;
        inD >> dwUserNum;
        ui->listWidget->clear();
        friendList.clear();
        for (quint32 i=0; i < dwUserNum; i++)
        {
            UserInfo stru;
            inD >> stru.szUsername;
            inD >> stru.shFlag;
            if (stru.shFlag)
            {
                inD >> stru.szIp;
                inD >> stru.sockd;
            }
            ui->listWidget->addItem(stru.szUsername + (stru.shFlag?tr(" [+]"):tr("")));
            friendList.push_back(stru);
        }
    }
}
