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
    connect(server, SIGNAL(signalDisconnected(qintptr)), this, SLOT(slotDisconnected(qintptr)));

}

void LCServer::slotDealMsg(qintptr sockd)
{
    QByteArray ba = server->getData(sockd);
    if (ba.size())
    {
        QDataStream d(&ba, QIODevice::ReadOnly);
        quint16 shPdu;
        d >> shPdu;
        char *outbuf = new char[MAX_OUTBUF_SIZE];
        uint outlen = 0;
        qDebug() << "void LCServer::slotDealMsg(qintptr sockd) - PDU=" << shPdu << ", sockd=" << sockd;
        char *inbuf = ba.data() + sizeof (shPdu);
        uint inlen = (uint)(ba.size() - sizeof (shPdu));
        switch (shPdu)
        {
        case LCDB_UserLogin_Rep_FromCli:
        {
            qintptr tmpSockd = sockd;
            qint16 shTmpRet = dbCtrl->DBCUserLogin(inbuf, inlen , outbuf, outlen, tmpSockd);
            if (LCDB_ERR_USER_Online == shTmpRet)
            {
                char *outb = NULL;
                uint outl = 0;
                server->slotSendMsg(tmpSockd, outb, outl, LCDB_KickUser_Req_ToCli);
            }
            else if (LCDB_ERR_SUCCESS == shTmpRet)
            {
                server->slotSendMsg(sockd, outbuf, outlen, LCDB_UserLogin_Rsp_ToCli);
                server->slotSendMsg(sockd, outbuf, outlen, LCDB_UserLogin_Rsp_ToCli);
                char *outb = new char[MAX_OUTBUF_SIZE];
                uint outl = 0;
                dbCtrl->DBCGetFriendList(sockd, outb, outl);
                server->slotSendMsg(sockd, outb, outl, LCDB_GetFriendList_Rsp_ToCli);
                qDebug() << "11111111111111111";
                QList<qintptr> sockList;
                dbCtrl->getSockds(outb, outl, sockList);
                QList<qintptr>::iterator sockIt;
                for (sockIt=sockList.begin(); sockIt!=sockList.end(); sockIt++)
                {
                    qDebug() << "222222222222222222222";
                    if (LCDB_ERR_SUCCESS == dbCtrl->DBCGetFriendList(*sockIt, outb, outl))
                            server->slotSendMsg(*sockIt, outb, outl, LCDB_GetFriendList_Rsp_ToCli);
                }

                delete outb;
                delete outbuf;
                return ;
            }
            shPdu = LCDB_UserLogin_Rsp_ToCli;
            break;
        }
        case LCDB_GetFriendList_Rep_FromCli:
        {
            dbCtrl->DBCGetFriendList(inbuf, inlen, outbuf, outlen);
            shPdu = LCDB_GetFriendList_Rsp_ToCli;
            break;
        }

        default:
        {
            delete outbuf;
            return;
        }
        }
        server->slotSendMsg(sockd, outbuf, outlen, shPdu);
        delete outbuf;
    }
}


void LCServer::slotDisconnected(qintptr sockd)
{
    qDebug() << "LCServer::slotDisconnected()";
    dbCtrl->DBCUserLogout(sockd);
}
