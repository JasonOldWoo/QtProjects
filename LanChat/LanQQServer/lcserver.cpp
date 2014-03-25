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
    lcdb = new LanCDB;
    if (LCDB_ERR_SUCCESS != lcdb->initialize())
    {
        qDebug() << "Database initialzation error!";
        delete lcdb;
        return ;
    }
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
    connect(server, SIGNAL(signalDisconnected(qintptr, QString)), this, SLOT(slotDisconnected(qintptr, QString)));

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
            qint16 shTmpRet = userLogin(inbuf, inlen , outbuf, outlen, tmpSockd);

            if (LCDB_ERR_USER_Online == shTmpRet)
            {
                char *outb = NULL;
                uint outl = 0;
                server->slotSendMsg(tmpSockd, outb, outl, LCDB_KickUser_Req_ToCli);
            }
            shPdu = LCDB_UserLogin_Rsp_ToCli;
            break;
        }
        case LCDB_GetFriendList_Rep_FromCli:
        {
            getFriendList(inbuf, inlen, outbuf, outlen);
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


void LCServer::slotDisconnected(qintptr sockd, QString szClientName)
{
    qDebug() << "LCServer::slotDisconnected(), sockd=" << sockd << ", clientName=" << szClientName;
    if (szClientName != "NULL")
        lcdb->updateOffUser(szClientName);
}

int LCServer::userLogin(const char *inbuf, uint inlen, char *&outbuf, uint &outlen, qintptr& sockd)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    UserInfo stru;
    qint16 shRet;

    d >> stru.szUsername;
    d >> stru.szUserPwd;
    d >> stru.dwUserType;
    stru.sockd = sockd;

    qintptr tmpSockd = 0;

    shRet = lcdb->verifyUser(stru);

    if (LCDB_ERR_SUCCESS == shRet)
    {

        if ((qintptr)(-1) != (tmpSockd=server->getSockdViaName(stru.szUsername)))
        {
            qDebug() << "LCServer::userLogin() - user has been online";
            sockd = tmpSockd;
            shRet = LCDB_ERR_USER_Online;
        }
        else
            lcdb->updateUserInfo(stru);
    }

    QByteArray outBa;
    QDataStream outD(&outBa, QIODevice::ReadWrite);
    outD << shRet;
    if (LCDB_ERR_SUCCESS == shRet)
    {
        outD << stru.dwUserId;
        server->setClientUsername(sockd, stru.szUsername);
    }
    outlen = outBa.size();
    if (outlen > MAX_OUTBUF_SIZE)
    {
        delete [](char *)outbuf;
        outbuf = NULL;
        outbuf = new char[outlen];
    }
    memcpy(outbuf, outBa.data(), outlen);
    qDebug() << "int LCDBCtrl::DBCUserLogin() - shRet=[" << shRet << "], outlen=[" << outlen << "]";
    return shRet;
}

int LCServer::getFriendList(const char *inbuf, uint inlen, char *&outbuf, uint &outlen)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    quint32 dwUserId;
    quint32 dwUserNum = 0;
    qint16 shRet;
    UserInfoList strus;
    strus.clear();

    d >> dwUserId;
    shRet = lcdb->getFriendList(dwUserId, dwUserNum, strus);

    QByteArray outBa;
    QDataStream outD(&outBa, QIODevice::ReadWrite);
    outD << shRet;
    if (LCDB_ERR_SUCCESS == shRet)
    {
        outD << dwUserNum;
        for (int i=0; i<dwUserNum; i++)
        {
            quint16 shFlag;
            outD << strus[i].szUsername;
            QString addr = server->getIpViaName(strus[i].szUsername);
            if ("NULL" == addr)
            {
                shFlag = 0;
                outD << shFlag;
            }
            else
            {
                shFlag = 1;
                outD << shFlag;
                outD << addr;
            }
        }
    }
    outlen = outBa.size();
    if (outlen > MAX_OUTBUF_SIZE)
    {
        delete [](char *)outbuf;
        outbuf = NULL;
        outbuf = new char[outlen];
    }
    memcpy(outbuf, outBa.data(), outlen);
    qDebug() << "int LCDBCtrl::DBCGetFriendList - shRet=[" << shRet << "], outlen=[" << outlen << "]";
    return shRet;
}
