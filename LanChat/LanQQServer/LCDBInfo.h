#ifndef LCDBINFO_H
#define LCDBINFO_H

#include <QtGlobal>
#include <QVector>

typedef struct LCDBUserInfo
{
    unsigned char byUserFlag;
    quint32 dwUserId;
    quint32 dwUserPwd;
    quint32 dwUserGroupId;
    quint32 dwUserType;
    char szUsername[129];
    char szIp[129];
}UserInfo;
typedef QVector<UserInfo> UserInfoList;
typedef QVector<UserInfo>::iterator UserInfoListIt;

#endif // LCDBINFO_H
