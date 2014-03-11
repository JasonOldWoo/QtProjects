#ifndef LANQQSERVER_H
#define LANQQSERVER_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include "server.h"
#include "tcpclientsocket.h"

namespace Ui {
class LanQQServer;
}

class LanQQServer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit LanQQServer(QWidget *parent = 0);
    ~LanQQServer();
    
private slots:
    void on_actionStart_Service_triggered();
    void slotUpdateMsg(QString);

signals:
    void signalUpdateMsg(QString);

private:
    Ui::LanQQServer *ui;
    Server *server;

};

#endif // LANQQSERVER_H
