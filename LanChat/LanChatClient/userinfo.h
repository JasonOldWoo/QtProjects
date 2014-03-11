#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QDataStream>

class UserInfo
{
    friend QDataStream& operator<<(QDataStream& s, const UserInfo& l);
    friend QDataStream& operator>>(QDataStream& s, UserInfo& l);
public:
    UserInfo();
    void setUserInfo(QString usrn, QString pswd, QString addr = "");
    const QString getUsername() const;
    const QString getPassword() const;
    const QString getAddress() const;

private:
    QString username;
    QString password;
    QString address;
};

#endif // USERINFO_H
