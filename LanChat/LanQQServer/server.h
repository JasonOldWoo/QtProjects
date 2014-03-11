#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include "tcpclientsocket.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0, short port = 8888);
    
signals:
    void signalUpdateMsg(QString);

public slots:
    void incomingConnection(qintptr handle);
    void slotReadMsg(QString msg);
    void slotDisconnected(qintptr);

private:
    QList<TcpClientSocket*>clientList;
    
};

#endif // SERVER_H
