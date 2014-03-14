#include "lcserver.h"
#include "ui_lcserver.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDataStream>

LCServer::LCServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LCServer)
{
    ui->setupUi(this);
    dbCtrl = new LCDBCtrl;
}

LCServer::~LCServer()
{
    delete ui;

}

void LCServer::on_actionStart_Service_triggered()
{
    ui->actionStart_Service->setEnabled(false);

    server = new Server();

    connect(server, SIGNAL(signalMsg(qintptr)), this, SLOT(slotDealMsg(qintptr)));

}

void LCServer::slotDealMsg(qintptr sockd)
{
    QByteArray ba = server->getData(sockd);
    qDebug() << "inlen=[" << ba.size() << "]";
    if (ba.size())
    {
        QDataStream d(&ba, QIODevice::ReadOnly);
        quint16 shPdu;
        d >> shPdu;
        char *outbuf = new char[MAX_OUTBUF_SIZE];
        uint outlen = 0;
        qDebug() << "PDU=[" << shPdu << "]";
        char *inbuf = ba.data() + sizeof (shPdu);
        uint inlen = (uint)(ba.size() - sizeof (shPdu));
        switch (shPdu)
        {
        case LCDB_UserLogin_Rep_FromCli:
            dbCtrl->DBCUserLogin(inbuf, inlen , outbuf, outlen);
            shPdu = LCDB_UserLogin_Rsp_ToCli;
            break;
        case LCDB_GetFriendList_Rep_FromCli:

        default:
            delete outbuf;
            return;
        }
        server->slotSendMsg(sockd, outbuf, outlen, shPdu);
        delete outbuf;
    }
}
