#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QMap>
#include "tcpclientsocket.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0, short port = 8888);
    QByteArray getData(qintptr sockd);
    void setClientUsername(qintptr sockd, char *szUsername, uint len);
    
signals:
    void signalMsg(qintptr sockd);
    void signalDisconnected(qintptr sockd);

public slots:
    void incomingConnection(qintptr handle);
    void slotReadMsg(qintptr sockd);
    void slotDisconnected(qintptr sockd);
    void slotSendMsg(qintptr sockd, char *outbuf, uint outlen, quint16 shPdu);
    void slotDisconnect(qintptr sockd);

private:
    QMap<qintptr, TcpClientSocket*>clientList;
    enum {SENDMSG, DISCONNECT, DISCONNECTED};
    
};

#endif // SERVER_H
