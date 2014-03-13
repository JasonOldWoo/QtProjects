#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClientSocket(QObject *parent = 0);

signals:
    void signalDisconnected(qintptr);
    void signalMsg(char *inbuf, uint inlen);

public slots:
    void slotReadData();
    void slotDisconneted();

private:

};

#endif // TCPCLIENTSOCKET_H
