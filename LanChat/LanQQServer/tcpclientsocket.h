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

signals:
    void signalDisconnected(qintptr);
    void signalMsg(qintptr);

public slots:
    void slotReadData();
    void slotDisconneted();

private:
    LinkQueue<QByteArray> inDataList;

};

#endif // TCPCLIENTSOCKET_H
