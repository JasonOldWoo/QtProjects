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
    connect(client, SIGNAL(signalDisconnected(qintptr)), this, SLOT(slotDisconnected(qintptr)));

    clientList.insert(handle, client);
}


void Server::slotReadMsg(qintptr sockd)
{
    emit signalMsg(sockd);
}

void Server::slotDisconnected(qintptr sockd)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    qDebug() << "void Server::slotDisconnected(qintptr sockd) - sockd=" << sockd << "disconnected";
    if (clientIt != clientList.end())
    {
        clientList.erase(clientIt);
    }
    emit signalDisconnected(sockd);
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

void Server::setClientUsername(qintptr sockd, char *szUsername, uint len)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        (clientIt.value())->setClientName(szUsername, len);
    }
}
