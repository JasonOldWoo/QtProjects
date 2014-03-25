#include "tcpclientsocket.h"
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>

TcpClientSocket::TcpClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), SLOT(slotReadData()));
    connect(this, SIGNAL(disconnected()), SLOT(slotDisconneted()));
    sockd = socketDescriptor();
    qDebug() << "QTcpClientSocket::QTcpClientSoket() - sockd=" << sockd << endl;
    m_szClientName = "NULL";
}


void TcpClientSocket::slotReadData()
{
    sockd = socketDescriptor();
    qDebug() << "TcpClientSocket::slotReadData() - sockd=" << sockd;
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
    qDebug() << "TcpClientSocket::slotDisconneted() - sockd=" << sockd;
    emit signalDisconnected(sockd, m_szClientName);
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

void TcpClientSocket::setClientName(QString szClientName)
{
    m_szClientName = szClientName;
}

QString TcpClientSocket::getClientName()
{
    return m_szClientName;
}
