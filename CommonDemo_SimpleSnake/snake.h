#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QMap>
#include <QPainter>
#include <QKeyEvent>
#include <QList>
#include <QTimer>
#include <QVector>
#include <QDataStream>
#include <QLabel>
#include <QSqlTableModel>
#include <QTableView>
#include <QQueue>
#include <time.h>
#include "mydb.h"

#define BUTT_WIDTH 100
#define BUTT_HEIGT 50

namespace Ui {
class Snake;
}

class Snake : public QWidget
{
    Q_OBJECT
    
public:
    explicit Snake(QWidget *parent = 0);
    ~Snake();
    void isLost();
    void eatFood();
    void initialize();

private:
    void setCurrentDir(int x, int y);
    void genFood();
    
private:


    Ui::Snake *ui;
    MyDB myDB;			// 数据库
    QRect snakeHead;		// 蛇头
    QList<QRect>snakeBody;	// 蛇身
    int nodeHeight;		// 节点宽度
    int nodeWidth;		// 节点高度
    QPoint velocityVector;	// 用户按键方向矢量
    QQueue<QPoint> vList;	// 按键队列
    QList<QRect>map;		// 障碍
    bool die;			// 蛇的生死
    QRect food;			// 食物
    int count;			// 吃到食物的次数
    //int timerFd;
    int timeVal;		// 计时器时间间隔
    int score;			// 分数
    int drawEvent;		// 当前的游戏状态，定义于gameStatus
    int currentDir;		// 当前蛇的前进方向
    bool flag;			// 用户长按建的标志，true 长按建
    bool speedFlag;		// 是否有速度升级，当用户游戏速度满
				// 足升级条件但是又处于加速状态，通
				// 过设定该标志来延迟速度的升级
    int extraPoint;		// 奖励分数
    int luck;			// 骰子，用来决定食物的颜色
    //time_t startTime;
    enum gameStaus{PLAY, DIE, HIGHSCORE, DISPHIGH};	// 游戏状态
    QTimer t;

    QPushButton continuePushButton;
    QPushButton confirmPushButton;
    QPushButton replayPushButton;
    QLineEdit nameLine;
    QTableView table;
    QLabel btLabel;


public slots:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *k);
    void keyReleaseEvent(QKeyEvent *k);
//    void timerEvent(QTimerEvent *);
    void qTimerEvent();
    void readHighScore();
    void saveRecordAndDispHigh();
    void replay();
};

#endif // SNAKE_H
