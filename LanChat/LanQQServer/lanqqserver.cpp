#include "lanqqserver.h"
#include "ui_lanqqserver.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDataStream>

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

    connect(server, SIGNAL(signalNewData(char *, uint)), this, SLOT(slotUpdateMsg(char *, uint)));

}

void LanQQServer::slotUpdateMsg(char *inbuf, uint inlen)
{
    qDebug() << "LanQQserver: new data length: " << inlen;
//    ui->msgListWidget->addItem(msg);
    QByteArray ba;
    QDataStream d(&ba, QIODevice::ReadOnly);
    unsigned short shPdu;
    d >> shPdu;

}
