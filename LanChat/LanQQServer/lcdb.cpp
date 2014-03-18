#include "lcdb.h"
#include <sstream>
#include <QDebug>
#include <string.h>

LanCDB::LanCDB()
{
}

int LanCDB::initialize()
{
    loadDBConfig();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(m_LCDBHost);
    db.setUserName(m_LCDBUsername);
    db.setPassword(m_LCDBPassword);
    db.setDatabaseName(m_LCDBName);
    db.setPort(m_LCDBPort);

    if (!db.open())
    {
        qDebug() << "Connect to MySql error!";
        qDebug() << db.lastError();
        return LCDB_ERR_DBDATA_ERROR;
    }
    qDebug() << "Connect to Mysql successfully!";
    return LCDB_ERR_SUCCESS;
}

void LanCDB::loadDBConfig()
{
    QFile configFile("LanChatServ.xml");
    if (!configFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Use default configuration.";
        m_LCDBHost = DEFAULT_DB_HOST;
        m_LCDBPassword = DEFAULT_DB_PASSWORD;
        m_LCDBPort = DEFAULT_DB_PORT;
        m_LCDBUsername = DEFAULT_DB_USERNAME;
        m_LCDBName = DEFAULT_DB_NAME;
        return ;

    }
//    QXmlStreamReader configReader(&configFile);
    /* default for now */
}

int LanCDB::verifyUser(char *szUsername, char *szPassword)
{
    std::stringstream oss;
    oss.str("");
    oss << "select user_id from users where user_name='";
    oss << szUsername << "' and aes_decrypt(user_pwd, 'users.user_pwd')='";
    oss << szPassword << "'";

    qDebug() << "SQL=[" << oss.str().c_str() << "]";

    QSqlQuery *sqlQuery = new QSqlQuery(db);
    if (!sqlQuery->exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return LCDB_ERR_DBDATA_ERROR;
    }

    if (sqlQuery->numRowsAffected())
        return LCDB_ERR_SUCCESS;
    else
        return LCDB_ERR_USER_VerifyFailed;
}

int LanCDB::updateLoginTime(char *szUsername)
{
    std::stringstream oss;
    oss.str("");
    oss << "update users set login_time=current_timestamp, logout_time='0000-00-00 00:00:00' where user_name='" << szUsername << "'";

    QSqlQuery *sqlQuery = new QSqlQuery(db);
    if (!sqlQuery->exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return LCDB_ERR_DBDATA_ERROR;
    }

    if (sqlQuery->numRowsAffected())
        return LCDB_ERR_SUCCESS;
    else
        return LCDB_ERR_USER_NotExist;
}

int LanCDB::getFriendList(quint32 dwUserId, quint32& dwUserNum, UserInfoList& strus)
{
    std::stringstream oss;
    oss.str("");
    oss << "select user_name from users where user_id in(select user_id_b from userGrant where deal_flag=1 and user_id_a=" << dwUserId;
    oss << " union select user_id_a from userGrant where deal_flag=1 and user_id_b=" << dwUserId << ")";

    qDebug() << "SQL=[" << oss.str().c_str() << "]";

    QSqlQuery *sqlQuery = new QSqlQuery(db);
    if (!sqlQuery->exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return LCDB_ERR_DBDATA_ERROR;
    }

    dwUserNum = 0;
    if (sqlQuery->numRowsAffected())
    {
        while (sqlQuery->next())
        {
            UserInfo stru;
            memset(&stru, 0, sizeof (stru));
            memcpy(stru.szUsername, (char *)sqlQuery->value(0).data());
            strus.push_back(stru);
            dwUserNum++;
        }
    }
}
