#include "lcdbctrl.h"

LCDBCtrl::LCDBCtrl()
{
    dbc.initialize();
}

int LCDBCtrl::DBCUserLogin(const char *inbuf, const uint inlen, char *&outbuf, uint &outlen, qintptr &sockd)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    UserInfo stru;
    qint16 shRet;

    d >> stru.szUsername;
    d >> stru.szUserPwd;
    d >> stru.dwUserType;
    d >> stru.szIp;
    stru.sockd = sockd;

    shRet = dbc.verifyUser(stru, sockd);

    if (1 == stru.shFlag && LCDB_ERR_SUCCESS == shRet)
        shRet = LCDB_ERR_USER_Online;

    else if (LCDB_ERR_SUCCESS == shRet)
        dbc.updateUserInfo(stru);

    QByteArray outBa;
    QDataStream outD(&outBa, QIODevice::ReadWrite);
    outD << shRet;
    if (LCDB_ERR_SUCCESS == shRet)
        outD << stru.dwUserId;
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

int LCDBCtrl::DBCUserRegister(const char *inbuf, const uint inlen, char *&outbuf, uint &outlen)
{
}

int LCDBCtrl::DBCGetFriendList(const char *inbuf, const uint inlen, char *&outbuf, uint &outlen)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    quint32 dwUserId;
    quint32 dwUserNum = 0;
    qint16 shRet;
    UserInfoList strus;
    strus.clear();

    d >> dwUserId;
    shRet = dbc.getFriendList(dwUserId, dwUserNum, strus);

    QByteArray outBa;
    QDataStream outD(&outBa, QIODevice::ReadWrite);
    outD << shRet;
    if (LCDB_ERR_SUCCESS == shRet)
    {
        outD << dwUserNum;
        for (int i=0; i<dwUserNum; i++)
        {
            outD << strus[i].szUsername;
            outD << strus[i].shFlag;
            if (strus[i].shFlag)
            {
                outD << strus[i].szIp;
                outD << strus[i].sockd;
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

int LCDBCtrl::DBCGetFriendList(qintptr sockd, char *&outbuf, uint &outlen)
{
    quint32 dwUserId = 0;
    qint16 shRet = dbc.getUserIdViaSockd(sockd, dwUserId);
    qDebug() << "int LCDBCtrl::DBCGetFriendList(qintptr sockd, char *&outbuf, uint &outlen), shRet=" << shRet;
    if (LCDB_ERR_SUCCESS == shRet)
    {
        QByteArray inBa;
        QDataStream inD(&inBa, QIODevice::ReadWrite);
        inD << dwUserId;
        return DBCGetFriendList(inBa.data(), inBa.length(), outbuf, outlen);
    }
    return shRet;
}

int LCDBCtrl::DBCUserLogout(qintptr sockd)
{
    return dbc.updateOffUser(sockd);
}

void LCDBCtrl::getSockds(const char *inbuf, const uint inlen, QList<qintptr> &sockList)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    quint16 shRet;
    quint32 dwUserNum;
    d >> shRet;

    for (quint32 i=0; i<dwUserNum; i++)
    {
        UserInfo stru;
        qintptr sockd;
        d >> stru.szUsername;
        d >> stru.shFlag;
        if (stru.shFlag)
        {
            d >> stru.szIp;
            d >> sockd;
            sockList.push_back(sockd);
        }
    }
}
