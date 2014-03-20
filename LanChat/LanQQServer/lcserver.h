#ifndef LANQQSERVER_H
#define LANQQSERVER_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include "tcpserver.h"
#include "tcpclientsocket.h"
#include "LCPdu.h"
#include "lcdbctrl.h"

namespace Ui {
class LCServer;
}

class LCServer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit LCServer(QWidget *parent = 0);
    ~LCServer();

    
private slots:
    void on_actionStart_Service_triggered();
    void slotDealMsg(qintptr sockd);
    void slotDisconnected(qintptr sockd);

private:
    Ui::LCServer *ui;
    Server *server;
    LCDBCtrl *dbCtrl;
};

#endif // LANQQSERVER_H
