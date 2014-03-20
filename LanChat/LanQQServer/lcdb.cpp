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

    std::stringstream oss;
    oss.str("");
    oss << "update users set user_flag=0, user_sockd=0";
    QSqlQuery *sqlQuery = new QSqlQuery(db);
    if (!sqlQuery->exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return LCDB_ERR_DBDATA_ERROR;
    }
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

int LanCDB::verifyUser(UserInfo &stru, qintptr &sockd)
{
    std::stringstream oss;
    oss.str("");
    oss << "select user_id, user_flag, user_sockd from users where user_name='";
    oss << stru.szUsername.toUtf8().data() << "' and aes_decrypt(user_pwd, 'users.user_pwd')='";
    oss << stru.szUserPwd.toUtf8().data() << "'";

    qDebug() << "SQL=[" << oss.str().c_str() << "]";

    QSqlQuery *sqlQuery = new QSqlQuery(db);
    if (!sqlQuery->exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return LCDB_ERR_DBDATA_ERROR;
    }

    if (sqlQuery->numRowsAffected())
    {
        if (sqlQuery->next())
        {
            stru.dwUserId = (quint32)sqlQuery->value(0).toInt();
            stru.shFlag = (quint16)sqlQuery->value(1).toInt();
            sockd = (qintptr)sqlQuery->value(2).toLongLong();
        }
        return LCDB_ERR_SUCCESS;
    }
    else
        return LCDB_ERR_USER_VerifyFailed;
}

int LanCDB::updateUserInfo(UserInfo &stru)
{
    std::stringstream oss;
    oss.str("");
    oss << "update users set user_ip='" << stru.szIp.toUtf8().data();
    oss << "', user_sockd=" << stru.sockd;
    oss << ", user_flag=1";
    oss << ", login_time=current_timestamp, logout_time='0000-00-00 00:00:00' where user_name='" << stru.szUsername.toUtf8().data() << "'";

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
        return LCDB_ERR_USER_NotExist;
}

int LanCDB::updateOffUser(qintptr sockd)
{
    std::stringstream oss;
    oss.str("");
    oss << "update users set logout_time=current_timestamp, user_flag=0 where user_sockd=" << sockd;
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
        return LCDB_ERR_USER_NotExist;
}

int LanCDB::getFriendList(quint32 dwUserId, quint32& dwUserNum, UserInfoList& strus)
{
    std::stringstream oss;
    oss.str("");
    oss << "select user_name, user_flag, user_ip, user_sockd from users where user_id in(select user_id_b from userGrant where deal_flag=1 and user_id_a=" << dwUserId;
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
            stru.szUsername = sqlQuery->value(0).toString();
            stru.shFlag = (qint16)sqlQuery->value(1).toInt();
            stru.szIp = sqlQuery->value(2).toString();
            stru.sockd = sqlQuery->value(3).toLongLong();
            strus.push_back(stru);
            dwUserNum++;
            qDebug() << "int LanCDB::getFriendList(quint32 dwUserId, quint32& dwUserNum, UserInfoList& strus), sockd=" << stru.sockd;
        }
    }
    return LCDB_ERR_SUCCESS;
}

int LanCDB::getUserIdViaSockd(qintptr sockd, quint32 &dwUserId)
{
    std::stringstream oss;
    oss.str("");
    oss << "select user_id from users where user_sockd=" << sockd;
    qDebug() << "SQL=[" << oss.str().c_str() << "]";
    QSqlQuery *sqlQuery = new QSqlQuery(db);
    if (!sqlQuery->exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return LCDB_ERR_DBDATA_ERROR;
    }

    if (sqlQuery->numRowsAffected())
    {
        if (sqlQuery->next())
        {
            dwUserId = (quint32)sqlQuery->value(0).toInt();
            qDebug() << "int LanCDB::getUserIdViaSockd(qintptr sockd, quint32 &dwUserId), dwUserId=" << dwUserId;
        }
    }
    else
        return LCDB_ERR_USER_NotExist;

    return LCDB_ERR_SUCCESS;
}
