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
    port = 8888;
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

    server = new Server(this, port);

    connect(server, SIGNAL(signalMsg(qintptr)), this, SLOT(slotDealMsg(qintptr)));
    connect(server, SIGNAL(signalDisconnected(qintptr, QString)), this, SLOT(slotDisconnected(qintptr, QString)));
    connect(this, SIGNAL(signalUserLogin(qintptr)), this, SLOT(slotNoticeClientUserLogin(qintptr)));

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
            else if (LCDB_ERR_SUCCESS ==  shTmpRet)
            {
                emit signalUserLogin(sockd);
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

        case LCDB_GroupChat_Rep_FromCli:
        {
            groupChat(inbuf, inlen, outbuf, outlen);
            shPdu = LCDB_GroupChat_Rsp_ToCli;
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
    {
        lcdb->updateOffUser(szClientName);
        slotNoticeClientUserLogout(szClientName);
    }
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

int LCServer::groupChat(const char *inbuf, uint inlen, char *&outbuf, uint &outlen)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    UserInfo stru;
    QString msg;
    quint32 dwGroupId;

    d >> stru.dwUserId;
    lcdb->getUsername(stru);
    d >> dwGroupId;
    d >> msg;

    QByteArray outBa;
    QDataStream outD();

    qDebug() << "int LCServer::groupChat() - username=" << stru.szUsername << ", group=" << dwGroupId;

    QUdpSocket *udpSocket = new QUdpSocket(this);
    qint64 len = udpSocket->writeDatagram(ba.data(), ba.length(), QHostAddress::LocalHost, port);
    if (len)
        qDebug() << "int LCServer::groupChat() - len=" << len;

    return LCDB_ERR_SUCCESS;
}

int LCServer::slotNoticeClientUserLogin(qintptr sockd)
{
    qDebug() << "\nint LCServer::slotNoticeClientUserLogin() sockd=" << sockd << "\n\n";
    QString szUsername = server->getNameViaSock(sockd);
    if ("NULL" == szUsername)
        return LCDB_ERR_USER_NotExist;
    UserInfoList strus;
    quint32 dwUserNum = 0;
    qint16 shRet;

    shRet = lcdb->getFriendList(szUsername, dwUserNum, strus);

    if (LCDB_ERR_SUCCESS == shRet && dwUserNum)
    {
        QByteArray outBa;
        QDataStream outD(&outBa, QIODevice::ReadWrite);

        quint16 shPdu = LCDB_NoticeClientUserLogin_Req_ToCli;
        outD << szUsername;

        for (quint32 i=0; i<dwUserNum; i++)
        {
            qDebug() << "int LCServer::slotNoticeClientUserLogin() - shFlag=" << strus[i].shFlag    \
                     << "clientname=" << strus[i].szUsername << ", username=" << szUsername;
            if (strus[i].shFlag)
            {
                server->slotSendMsg(strus[i].szUsername, outBa.data(), (uint)outBa.length(), shPdu);
            }
        }
    }
    return LCDB_ERR_SUCCESS;
}

int LCServer::slotNoticeClientUserLogout(QString szUsername)
{
    qDebug() << "\nint LCServer::slotNoticeClientUserLogout() szClientName" << szUsername << "\n\n";
    if ("NULL" == szUsername)
        return LCDB_ERR_USER_NotExist;
    UserInfoList strus;
    quint32 dwUserNum = 0;
    qint16 shRet;

    shRet = lcdb->getFriendList(szUsername, dwUserNum, strus);

    if (LCDB_ERR_SUCCESS == shRet && dwUserNum)
    {
        QByteArray outBa;
        QDataStream outD(&outBa, QIODevice::ReadWrite);

        quint16 shPdu = LCDB_NoticeClientUserLogout_Req_ToCli;
        outD << szUsername;

        for (quint32 i=0; i<dwUserNum; i++)
        {
            qDebug() << "int LCServer::slotNoticeClientUserLogout() - shFlag=" << strus[i].shFlag    \
                     << "clientname=" << strus[i].szUsername << ", username=" << szUsername;
            if (strus[i].shFlag)
            {
                server->slotSendMsg(strus[i].szUsername, outBa.data(), (uint)outBa.length(), shPdu);
            }
        }
    }
    return LCDB_ERR_SUCCESS;
}
