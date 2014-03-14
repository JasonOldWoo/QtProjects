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
    qDebug() << "TcpClientSocket::slotReadData()";
    while (bytesAvailable() > 0)
    {
        QByteArray ba = readAll();
        if (ba.length())
        {
            inDataList.enque(ba);
            emit signalMsg(socketDescriptor());
        }
    }
}

void TcpClientSocket::slotDisconneted()
{
    qDebug() << "TcpClientSocket::slotDisconneted()";
    emit signalDisconnected(socketDescriptor());
}

QByteArray TcpClientSocket::getData()
{
    qDebug() << "QByteArray TcpClientSocket::getData()";
    if (!inDataList.IsEmpty())
        return inDataList.deque();
    else
    {
        qDebug() << "QByteArray TcpClientSocket::getData() - is empty";
        QByteArray ba;
        ba.clear();
        return ba;
    }
}

