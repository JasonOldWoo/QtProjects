#include "mydb.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <sstream>
#include <QFile>
#include <QXmlStreamReader>
#if QT_VERSION <= QT_VERSION_CHECK(4,8,0)
#include <QVariant>
#endif

MyDB::MyDB()
{

}

MyDB::~MyDB()
{

}

int MyDB::initialize()
{
    loadDBConfig();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(m_snakeDBHost);
    db.setUserName(m_snakeDBUsername);
    db.setPassword(m_snakeDBPassword);
    db.setDatabaseName(m_snakeDBName);
    db.setPort(m_snakeDBPort);

    if (!db.open())
    {
        qDebug() << "Connect to MySql error!";
        qDebug() << db.lastError();
        return SNAKE_DB_ERR;
    }
    qDebug() << "Connect to Mysql successfully!";
    return SNAKE_DB_ERR_SUCCESS;
//    QSqlQuery sqlQuery = QSqlQuery(db);
//    sqlQuery.exec("insert into  score(user_score, user_id) values(192, 'Jason')");
//    qDebug() << sqlQuery.lastError().databaseText();
//    sqlQuery.clear();
//    if (sqlQuery.exec("select user_id from score"))
//    {
//        while(sqlQuery.next())
//            qDebug() << sqlQuery.value(0).toString();
//    }

}

int MyDB::isHighScore(unsigned long dwScore)
{
    QSqlQuery sqlQuery = QSqlQuery(db);
    std::stringstream oss;
    oss.str("");
    oss << "select distinct(user_index) from score where user_score < " << dwScore;
    if (!sqlQuery.exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return SNAKE_DB_ERR_DATA;
    }
    if (sqlQuery.numRowsAffected())
    {
	if (sqlQuery.next() && sqlQuery.value(0).toInt())
        {
            qDebug() << "new high score:" << dwScore;
            return SNAKE_DB_IS_NEWHIGH;
        }
    }
    else
    {
        oss.str("");
        oss << "select distinct(user_index) from score";
        if (!sqlQuery.exec((char *)(oss.str().c_str())))
        {
            qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
            qDebug() << db.lastError();
            return SNAKE_DB_ERR_DATA;
        }
        if (!sqlQuery.numRowsAffected() && dwScore)
        {
            qDebug() << "new high score:" << dwScore;
            return SNAKE_DB_IS_NEWHIGH;
        }
        else if (sqlQuery.numRowsAffected() < 10 && dwScore)
        {
            qDebug() << "new high score:" << dwScore;
            return SNAKE_DB_IS_NEWHIGH;
        }
        else
        {
            return SNAKE_DB_NOT_NEWHIGH;
        }
    }
}

int MyDB::saveRecord(char *szUsername, unsigned long dwScore)
{
    QSqlQuery sqlQuery = QSqlQuery(db);
    std::stringstream oss;
    unsigned long dwUserCount = 0;
    oss.str("");
    oss << "select count(user_index) from score";
    if (!sqlQuery.exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return SNAKE_DB_ERR_DATA;
    }
    if (sqlQuery.next())
        dwUserCount = (unsigned long)(sqlQuery.value(0).toString()).toLong();

    oss.str("");
    oss << "insert into score(user_name, user_score) values('";
    oss << szUsername << "',";
    oss << dwScore;
    oss << ")";
    if (!sqlQuery.exec((char *)(oss.str().c_str())))
    {
        qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
        qDebug() << db.lastError();
        return SNAKE_DB_ERR_DATA;
    }

    if (dwUserCount > 9)
    {
        oss.str("");
        oss << "select user_index from score order by user_score asc limit 1";
        if (!sqlQuery.exec((char *)(oss.str().c_str())))
        {
            qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
            qDebug() << db.lastError();
            return SNAKE_DB_ERR_DATA;
        }
        unsigned long dwUserIndex = 0;
        if (sqlQuery.next())
            dwUserIndex = sqlQuery.value(0).toString().toLong();

        oss.str("");
        oss << "delete from score where user_index=" << dwUserIndex;
        if (!sqlQuery.exec((char *)(oss.str().c_str())))
        {
            qDebug() << "Err_SQL=[" << oss.str().c_str() << "]";
            qDebug() << db.lastError();
            return SNAKE_DB_ERR_DATA;
        }
    }

    return SNAKE_DB_ERR_SUCCESS;
}

void MyDB::loadDBConfig()
{
    QFile configFile("snake_db.xml");
    if (!configFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Use default configuration.";
        m_snakeDBHost = DEFAULT_DB_HOST;
        m_snakeDBPassword = DEFAULT_DB_PASSWORD;
        m_snakeDBPort = DEFAULT_DB_PORT;
        m_snakeDBTable = DEFAULT_DB_TABLE;
        m_snakeDBUsername = DEFAULT_DB_USERNAME;
        m_snakeDBName = DEFAULT_DB_NAME;
        return ;
    }
//    QXmlStreamReader configReader(&configFile);
    /* default for now */
}
