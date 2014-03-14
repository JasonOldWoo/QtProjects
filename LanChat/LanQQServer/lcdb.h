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
    int verifyUser(char *szUsername, char *szPassword);
    int getFriendList(quint32 dwUserId, quint32 &dwUserNum, UserInfoList &strus);

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
