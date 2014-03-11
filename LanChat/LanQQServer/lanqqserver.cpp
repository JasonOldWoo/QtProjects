#include "lanqqserver.h"
#include "ui_lanqqserver.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDebug>

LanQQServer::LanQQServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LanQQServer)
{
    ui->setupUi(this);
}

LanQQServer::~LanQQServer()
{
    delete ui;

}

void LanQQServer::on_actionStart_Service_triggered()
{
    ui->actionStart_Service->setEnabled(false);

    server = new Server();

    connect(server, SIGNAL(signalUpdateMsg(QString)), this, SLOT(slotUpdateMsg(QString)));

}

void LanQQServer::slotUpdateMsg(QString msg)
{
    qDebug() << "LanQQserver: update msg: " << msg;
    ui->msgListWidget->addItem(msg);
}
