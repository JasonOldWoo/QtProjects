#include "lcdbctrl.h"

LCDBCtrl::LCDBCtrl()
{
    db.initialize();
}

int LCDBCtrl::DBCUserLogin(const char *inbuf, uint inlen, char *&outbuf, uint &outlen)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    QString szUsername;
    QString szUserPasswd;
    qint16 shRet;
    quint32 dwUserType;

    d >> szUsername;
    d >> szUserPasswd;
    d >> dwUserType;

    shRet = db.verifyUser(szUsername.toUtf8().data(), szUserPasswd.toUtf8().data());

    QByteArray outBa;
    QDataStream outD(&outBa, QIODevice::ReadWrite);
    outD << shRet;
    outlen = outBa.size();
    if (outlen > MAX_OUTBUF_SIZE)
    {
        delete [](char *)outbuf;
        outbuf = NULL;
        outbuf = new char[outlen];
    }
    memcpy(outbuf, outBa.data(), outlen);
    qDebug() << "int LCDBCtrl::userLogin() - shRet=[" << shRet << "], outlen=[" << outlen << "]";
    return shRet;
}

int LCDBCtrl::DBCUserRegister(const char *inbuf, uint inlen, char *&outbuf, uint &outlen)
{
}

int LCDBCtrl::DBCGetFriendList(const char *inbuf, uint inlen, char *&outbuf, uint &outlen)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    quint32 dwUserId;
    qint16 shRet;


}
