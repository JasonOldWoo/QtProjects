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
    if (clientIt != clientList.end())
        clientList.erase(clientIt);
}

void Server::slotSendMsg(qintptr sockd, char *outbuf, uint outlen)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
        (*clientIt)->write(outbuf, (qint64)outlen);
}

void Server::slotDisconnect(qintptr sockd)
{
    QMap<qintptr, TcpClientSocket*>::iterator  clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        (*clientIt)->close();
        clientList.erase(clientIt);
    }
}

void Server::slotDeal(qintptr sockd, qint8 flag, char *outbuf, uint outlen)
{
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        switch (flag)
        {
        case SENDMSG:
            if (outbuf)
                (*clientIt)->write(outbuf, (qint64)outlen);
            break;

        case DISCONNECT:
            clientList.erase(clientIt);
            break;

        case DISCONNECTED:
            (*clientIt)->close();
            clientList.erase(clientIt);
            break;

        default:
            break;
        }
    }
}

QByteArray Server::getData(qintptr sockd)
{
    qDebug() << "QByteArray Server::getData(qintptr sockd)";
    QMap<qintptr, TcpClientSocket*>::iterator clientIt;
    clientIt = clientList.find(sockd);
    if (clientIt != clientList.end())
    {
        return (*clientIt)->getData();
    }
    else
    {
        qDebug() << "QByteArray Server::getData(qintptr sockd) - is empty";
        QByteArray ba;
        ba.clear();
        return ba;
    }

}
