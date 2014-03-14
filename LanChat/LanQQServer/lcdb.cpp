#include "lcdb.h"
#include <sstream>

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
        m_LCDBTable = DEFAULT_DB_TABLE;
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
    oss << "select users where user_id='";
    oss << szUsername << "', user_pwd=aes_decrypt('";
    oss << szPassword << "', 'users.user_pwd')";
    qint16 shRet = -1;

    QSqlQuery *sqlQuery = new QSqlQuery(db);
    if (!sqlQuery.exec((char *)(oss.str().c_str())))
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
