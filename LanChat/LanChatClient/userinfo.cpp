#include "userinfo.h"

UserInfo::UserInfo()
{
}

void UserInfo::setUserInfo(QString usrn, QString pswd, QString addr)
{
    username = usrn;
    password = pswd;
    address = addr;
}

QDataStream& operator<<(QDataStream& s, const UserInfo& l)
{
    s << (qint32)l.username.length() << l.username
      << (qint32)l.password.length() << l.password
      << (qint32)l.address.length() << l.address;

    return s;
}

QDataStream& operator>>(QDataStream& s, UserInfo& l)
{
    qint32 num;
    s >> num >> l.username >> num >>l.password >> num >>l.address;

    return s;
}

const QString UserInfo::getUsername() const
{
    return username;
}

const QString UserInfo::getPassword() const
{
    return password;
}

const QString UserInfo::getAddress() const
{
    return address;
}
