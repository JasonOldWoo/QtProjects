#ifndef LANCDB_H
#define LANCDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include "LCPdu.h"

class LanCDB
{
public:
    LanCDB();
    int initialize();
    void loadDBConfig();
    int verifyUser(char *szUsername, char *szPassword);

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
