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
    void setClientName(QString szClientName);
    QString getClientName();

signals:
    void signalDisconnected(qintptr, QString);
    void signalMsg(qintptr);

public slots:
    void slotReadData();
    void slotDisconneted();

private:
    LinkQueue<QByteArray> inDataList;
    QString m_szClientName;
    qintptr sockd;

};

#endif // TCPCLIENTSOCKET_H
