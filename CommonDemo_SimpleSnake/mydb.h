#ifndef MYDB_H
#define MYDB_H
//#include <QSql>
#include <QSqlDatabase>
#include <QString>

#define SNAKE_DB_ERR_SUCCESS    0
#define SNAKE_ERR               -4000
#define SNAKE_DB_ERR            -4001
#define SNAKE_DB_ERR_DATA       -4002

#define SNAKE_DB_IS_NEWHIGH     2
#define SNAKE_DB_NOT_NEWHIGH    3

#define DEFAULT_DB_USERNAME     "snakeuser"
#define DEFAULT_DB_PASSWORD     "snake.webcon"
#define DEFAULT_DB_HOST         "localhost"
#define DEFAULT_DB_PORT         3306
#define DEFAULT_DB_NAME         "snake_db"
#define DEFAULT_DB_TABLE        "score"

class MyDB
{
public:
    MyDB();
    ~MyDB();
    int initialize();
    int isHighScore(unsigned long dwScore);
    int saveRecord(char *szUsername, unsigned long dwScore);

private:
    QSqlDatabase db;
    QString m_snakeDBUsername;
    QString m_snakeDBPassword;
    QString m_snakeDBHost;
    QString m_snakeDBTable;
    QString m_snakeDBName;
    unsigned short m_snakeDBPort;

private:
    void loadDBConfig();
};

#endif // MYDB_H
