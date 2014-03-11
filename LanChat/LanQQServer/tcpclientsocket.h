#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include <QFile>
#include <QDataStream>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClientSocket(QObject *parent = 0);
    bool Authenticate();
    
signals:
    void signalDisconnected(qintptr);
    void signalMsg(QString);
    
public slots:
    void slotReadData();
    void slotDisconneted();

private:
    bool isAutentication;
    
};

#endif // TCPCLIENTSOCKET_H
