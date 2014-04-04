#ifndef LANQQSERVER_H
#define LANQQSERVER_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QUdpSocket>
#include "tcpserver.h"
#include "tcpclientsocket.h"
#include "LCPdu.h"
#include "lcdb.h"

namespace Ui {
class LCServer;
}

class LCServer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit LCServer(QWidget *parent = 0);
    ~LCServer();
    int userLogin(const char *inbuf, uint inlen, char *&outbuf, uint &outlen, qintptr &sockd);
    int getFriendList(const char *inbuf, uint inlen, char *&outbuf, uint &outlen);
    int groupChat(const char *inbuf, uint inlen, char *&outbuf, uint &outlen);

    
private slots:
    void on_actionStart_Service_triggered();
    void slotDealMsg(qintptr sockd);
    void slotDisconnected(qintptr sockd, QString szClientName);
    int slotNoticeClientUserLogin(qintptr sockd);
    int slotNoticeClientUserLogout(QString szUsername);

signals:
    void signalUserLogin(qintptr);

private:
    Ui::LCServer *ui;
    Server *server;
    LanCDB *lcdb;
    short port;
};

#endif // LANQQSERVER_H
