#include "tcpclientsocket.h"
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>

TcpClientSocket::TcpClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), SLOT(slotReadData()));
    connect(this, SIGNAL(disconnected()), SLOT(slotDisconneted()));
}


void TcpClientSocket::slotReadData()
{
    while (bytesAvailable() > 0)
    {
        QByteArray ba = readAll();

        emit signalMsg(ba.data(), (uint)ba.length());
    }
}

void TcpClientSocket::slotDisconneted()
{
    qDebug() << "disconnect";
    emit signalDisconnected(socketDescriptor());
}
