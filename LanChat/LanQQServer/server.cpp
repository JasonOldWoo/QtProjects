#include "server.h"
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

    connect(client, SIGNAL(signalMsg(char *, uint)), this, SLOT(slotReadMsg(char *, uint)));
    connect(client, SIGNAL(signalDisconnected(qintptr)), this, SLOT(slotDisconnected(qintptr)));

    clientList.append(client);
}


void Server::slotReadMsg(char *inbuf, uint inlen)
{
    emit signalNewData(inbuf, inlen);

    foreach(TcpClientSocket *clt, clientList)
    {
        clt->write(inbuf, inlen);
        //QTextStream out(clt);
        //out << msg;

    }
}

void Server::slotDisconnected(qintptr description)
{
    for (int i=0; i<clientList.count(); i++)
    {
        if (clientList.at(i)->socketDescriptor() == description)
        {
            clientList.removeAt(i);
        }
    }
}
