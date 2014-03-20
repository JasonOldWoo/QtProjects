#ifndef LCDBINFO_H
#define LCDBINFO_H

#include <QtGlobal>
#include <QVector>

typedef struct LCDBUserInfo
{
    unsigned char byUserFlag;
    quint16 shFlag;             // 0:offline; 1:online
    quint32 dwUserId;
    QString szUserPwd;
    quint32 dwUserGroupId;
    quint32 dwUserType;
    QString szUsername;
    QString szIp;
    qintptr sockd;
}UserInfo;
typedef QVector<UserInfo> UserInfoList;
typedef QVector<UserInfo>::iterator UserInfoListIt;

#endif // LCDBINFO_H
