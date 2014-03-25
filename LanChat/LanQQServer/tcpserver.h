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
    void setClientUsername(qintptr sockd, QString szClientName);
    qintptr getSockdViaName(const QString szClientName);
    QString getIpViaSock(qintptr sockd);
    QString getIpViaName(QString szClientName);
    QString getNameViaSock(qintptr sockd);

    
signals:
    void signalMsg(qintptr sockd);
    void signalDisconnected(qintptr sockd, QString szClientName);

public slots:
    void incomingConnection(qintptr handle);
    void slotReadMsg(qintptr sockd);
    void slotDisconnected(qintptr sockd, QString szClientName);
    void slotSendMsg(qintptr sockd, char *outbuf, uint outlen, quint16 shPdu);

private:
    QMap<qintptr, TcpClientSocket*>clientList;
    enum {SENDMSG, DISCONNECT, DISCONNECTED};
    
};

#endif // SERVER_H
