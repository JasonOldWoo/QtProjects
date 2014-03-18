#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include "DArrQueue.h"

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClientSocket(QObject *parent = 0);
    QByteArray getData();
    void setClientName(const char *szUsername, uint len);
    const char *getClientName();

signals:
    void signalDisconnected(qintptr);
    void signalMsg(qintptr);

public slots:
    void slotReadData();
    void slotDisconneted();

private:
    LinkQueue<QByteArray> inDataList;
    char m_szClientName[129];

};

#endif // TCPCLIENTSOCKET_H
