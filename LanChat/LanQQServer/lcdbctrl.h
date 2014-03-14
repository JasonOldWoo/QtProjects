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
    int DBCUserLogin(const char *inbuf, uint inlen, char *&outbuf, uint &outlen);  // login
    int DBCUserRegister(const char *inbuf, uint inlen, char *&outbuf, uint &outlen);   // sign up fro lanchat
    int DBCGetFriendList(const char *inbuf, uint inlen, char *&outbuf, uint &outlen);  // fetch friends of specific user
    int DBCGetUserList(const char *inbuf, uint inlen, char *&outbuf, uint &outlen);    // fetch all users

private:
    LanCDB db;
};

#endif // LCDBCTRL_H
