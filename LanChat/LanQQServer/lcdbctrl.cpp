#include "lcdbctrl.h"

LCDBCtrl::LCDBCtrl()
{
}

int LCDBCtrl::userLogin(const char *inbuf, uint inlen, char *&outbuf, uint &outlen)
{
    QByteArray ba((char *)inbuf, inlen);
    QDataStream d(&ba, QIODevice::ReadOnly);
    QString szUserId;
    QString szUserPasswd;
    qint16 shRet;
    quint32 dwUserType;

    d >> szUserId;
    d >> szUserPasswd;
    d >> dwUserType;

    shRet = db.verifyUser(szUserId, szUserPasswd);

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

int LCDBCtrl::userRegister(const char *inbuf, uint inlen, char *&outbuf, uint &outlen)
{
}
