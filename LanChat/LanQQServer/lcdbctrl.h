#ifndef LCDBCTRL_H
#define LCDBCTRL_H
#include "lcdb.h"
#include <QDataStream>
#include <QByteArray>
#include <QIODevice>
#include <QString>
#include <QDebug>
#include "LCPdu.h"

class LCDBCtrl
{
public:
    LCDBCtrl();
    int DBCUserLogin(const char *inbuf, const uint inlen, char *&outbuf, uint &outlen, qintptr &sockd);  // login
    int DBCUserRegister(const char *inbuf, const uint inlen, char *&outbuf, uint &outlen);   // sign up fro lanchat
    int DBCGetFriendList(const char *inbuf, const uint inlen, char *&outbuf, uint &outlen);  // fetch friends of specific user
    int DBCGetFriendList(qintptr sockd, char *&outbuf, uint &outlen);
    int DBCGetUserList(const char *inbuf, const uint inlen, char *&outbuf, uint &outlen);    // fetch all users
    int DBCUserLogout(qintptr sockd);
    void getSockds(const char *inbuf, const uint inlen, QList<qintptr> &sockList);

private:
    LanCDB dbc;
};

#endif // LCDBCTRL_H
