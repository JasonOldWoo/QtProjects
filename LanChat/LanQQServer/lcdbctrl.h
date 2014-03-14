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
    int userLogin(const char *inbuf, uint inlen, char *&outbuf, uint &outlen);
    int userRegister(const char *inbuf, uint inlen, char *&outbuf, uint &outlen);

private:
    LanCDB db;
};

#endif // LCDBCTRL_H
