#include "tcpclientsocket.h"
#include <QHostAddress>

TcpClientSocket::TcpClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
    isAutentication = true;
    connect(this, SIGNAL(readyRead()), SLOT(slotReadData()));
    connect(this, SIGNAL(disconnected()), SLOT(slotDisconneted()));
}


void TcpClientSocket::slotReadData()
{
    if (isAutentication)
    {
        Authenticate();
    }
    while (bytesAvailable() > 0)
    {
        QTextStream in(this);
        QString msg;
        in >> msg;

        emit signalMsg(peerAddress().toString() + ":" + msg);
    }
}

void TcpClientSocket::slotDisconneted()
{
    qDebug() << "disconnect";
    emit signalDisconnected(socketDescriptor());
}

bool TcpClientSocket::Authenticate()
{

}


