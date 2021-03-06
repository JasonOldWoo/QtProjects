#ifndef LANCDB_H
#define LANCDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QSqlError>
#include "LCPdu.h"
#include "LCDBInfo.h"


#define DEFAULT_DB_USERNAME     "lancuser"
#define DEFAULT_DB_PASSWORD     "lanc.webcon"
#define DEFAULT_DB_HOST         "localhost"
#define DEFAULT_DB_PORT         3306
#define DEFAULT_DB_NAME         "lanc_db"

class LanCDB
{
public:
    LanCDB();
    int initialize();
    void loadDBConfig();
    int verifyUser(UserInfo &stru);
    int getFriendList(quint32 dwUserId, quint32 &dwUserNum, UserInfoList &strus);
    int getFriendList(QString szUsername, quint32 &dwUserNum, UserInfoList &strus);
    int getUserIdViaSockd(qintptr sockd, quint32 &dwUserId);
    int updateUserInfo(UserInfo &stru);
    int updateOffUser(QString szUsername);
    int getUsername(UserInfo& stru);

private:
    QSqlDatabase db;
    QString m_LCDBUsername;
    QString m_LCDBPassword;
    QString m_LCDBHost;
    QString m_LCDBTable;
    QString m_LCDBName;
    unsigned short m_LCDBPort;
};

#endif // LANCDB_H
