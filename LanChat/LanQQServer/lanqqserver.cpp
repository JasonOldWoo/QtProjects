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

    connect(server, SIGNAL(signalMsg(qintptr)), this, SLOT(slotDealMsg(qintptr)));

}

void LanQQServer::slotDealMsg(qintptr sockd)
{
    QByteArray ba = server->getData(sockd);
    qDebug() << "inlen=[" << ba.size() << "]";
    if (ba.size())
    {
        QDataStream d(&ba, QIODevice::ReadOnly);
        unsigned short shPdu;
        d >> shPdu;
        qDebug() << "PDU=[" << shPdu << "]";
        switch (shPdu)
        {
        }
    }
}
