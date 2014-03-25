#include "tcpserver.h"
#include "tcpclientsocket.h"

Server::Server(QObject *parent, short port) :
    QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
    qDebug() << "listen on socket, port: " << port;
}


void Server::incomingConnection(qintptr handle)
{
    TcpClientSocket *client = new TcpClientSocket(this);
    client->setSocketDescriptor(handle);

    connect(client, SIGNAL(signalMsg(qintptr)), this, SLOT(slotReadMsg(qintptr)));
    connect(client, SIGNAL(signalDisconnected(qintptr, QString)), this, SLOT(slotDisconnected(qintptr, QString)));

    clientList.insert(handle, client);
}


void Server::slotReadMsg(qintptr sockd)
{
    emit signalMsg(sockd);
}

void Server::slotDisconnected(qintptr sockd, QString szClientName)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        qDebug() << "void Server::slotDisconnected() - sockd=" << sockd << ", clientName=" << szClientName << " disconnected";
        clientList.erase(clientIt);
    }
    emit signalDisconnected(sockd, szClientName);
}

void Server::slotSendMsg(qintptr sockd, char *outbuf, uint outlen, quint16 shPdu)
{
    QByteArray outBa;
    QDataStream outD(&outBa, QIODevice::ReadWrite);
    outD << shPdu;
    outD.writeRawData(outbuf, outlen);
    outlen = outBa.length();
    QDataStream chkD(&outBa, QIODevice::ReadOnly);
    qint16 shRet;
    chkD >> shPdu;
    chkD >> shRet;

    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        while(!(clientIt.value())->isWritable())
            ;
        (clientIt.value())->write(outBa.data(), (qint64)outlen);
        qDebug() << "void Server::slotSendMsg() - outlen=[" << outlen << "], sockd=" << sockd << " ,shPdu=" << shPdu << " ,shRet" << shRet;
    }
}


QByteArray Server::getData(qintptr sockd)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        qDebug() << "QByteArray Server::getData(qintptr sockd) - sockd=" << sockd;
        return (clientIt.value())->getData();
    }
    else
    {
        qDebug() << "QByteArray Server::getData(qintptr sockd) - is empty, sockd=" << sockd;
        QByteArray ba;
        ba.clear();
        return ba;
    }
}

void Server::setClientUsername(qintptr sockd, QString szClientName)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        qDebug() << "void Server::setClientUsername() - sockd=" << sockd << ", szClientName=" << szClientName;
        (clientIt.value())->setClientName(szClientName);
    }
}

QString Server::getIpViaSock(qintptr sockd)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        QHostAddress hAddr = (clientIt.value())->peerAddress();
        if (hAddr.isNull())
        {
            QString addr("NULL");
            return addr;
        }
        else
            return hAddr.toString();
    }
    QString addr("NULL");
    return addr;
}

QString Server::getIpViaName(QString szClientName)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    for (clientIt=clientList.begin(); clientIt!=clientList.end(); clientIt++)
    {
        if (szClientName == (clientIt.value())->getClientName())
        {
            QHostAddress hAddr = (clientIt.value())->peerAddress();
            if (hAddr.isNull())
            {
                QString addr("NULL");
                return addr;
            }
            else
                return hAddr.toString();
        }
    }
    QString addr("NULL");
    return addr;
}

qintptr Server::getSockdViaName(const QString szClientName)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    for (clientIt=clientList.begin(); clientIt!=clientList.end(); clientIt++)
    {
        if (szClientName == (clientIt.value())->getClientName())
        {
            qDebug() << "qintptr Server::getSockdViaName() - szClientName=" << szClientName << " ,sockd=" << (clientIt.value())->socketDescriptor();
            return (clientIt.value())->socketDescriptor();
        }
    }
    return (qintptr)(-1);
}

QString Server::getNameViaSock(qintptr sockd)
{
    qDebug() << "QString Server::getNameViaSock() - sockd=" << sockd;
    QString retName = "NULL";
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        retName = (clientIt.value())->getClientName();
        qDebug() << "QString Server::getNameViaSock() - sockd=" << sockd << ", clientName=" << retName;
    }

    return retName;
}
