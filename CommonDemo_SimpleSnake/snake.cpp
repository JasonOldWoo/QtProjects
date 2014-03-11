#include "snake.h"
#include "ui_snake.h"
#include <QPainter>
#include <QRegion>
//#include <QtMath>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QTableView>

Snake::Snake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Snake)
{
    if (SNAKE_DB_ERR == myDB.initialize())
        return;
    ui->setupUi(this);
    setMaximumHeight(520);
    setMaximumWidth(500);
    setMinimumHeight(520);
    setMinimumWidth(500);


    connect(&continuePushButton, SIGNAL(clicked()), this, SLOT(readHighScore()));	// 点击该按钮，读取游戏最高分
    connect(&confirmPushButton, SIGNAL(clicked()), this, SLOT(saveRecordAndDispHigh()));	// 点击该按钮，保存最高分并显示
    connect(&replayPushButton, SIGNAL(clicked()), this, SLOT(replay()));	// 点击改按钮重新游戏

    initialize();

    connect(&t, SIGNAL(timeout()), this, SLOT(qTimerEvent()));	// 连接计时器和时间处理函数，当计时器过一个时间间隔时，
								// 发射timeout()信号，触发qTimerEvent槽函数
}

/* 初始化界面 */
void Snake::initialize()
{
    nodeHeight = 10;
    nodeWidth = 10;

    /* 初始化蛇 */
    snakeBody.clear();
    QRect rect1(250, 250, nodeWidth, nodeHeight);
    QRect rect2(250, 250+nodeHeight, nodeWidth, nodeHeight);
    QRect rect3(250, 250+2*nodeHeight, nodeWidth, nodeHeight);
    QRect rect4(250, 250+3*nodeHeight, nodeWidth, nodeHeight);
    QRect rect5(250, 250+4*nodeHeight, nodeWidth, nodeHeight);
    snakeBody.push_back(rect1);
    snakeBody.push_back(rect2);
    snakeBody.push_back(rect3);
    snakeBody.push_back(rect4);
    snakeBody.push_back(rect5);
    snakeHead = rect1;
    currentDir = Qt::Key_Up;

    /* 初始化食物 */
    genFood();

    /* 初始化游戏参数 */
    die = false;
    count = 0;
    score = 0;
    drawEvent = PLAY;
    timeVal = 200;
    flag = true;
    speedFlag = false;
    luck = 0;
    extraPoint = 0;

    vList.clear();
    velocityVector.setX(0);
    velocityVector.setY(-nodeHeight);
    vList.enqueue(velocityVector);

    /* 初始化地图 */
    int i;
    for (i=0; i<50; i++)
    {
        QRect mapRect(0, i*nodeHeight, nodeWidth, nodeHeight);
        map.push_back(mapRect);
    }
    for (i=1; i<49; i++)
    {
        QRect mapRect(i*nodeWidth, 0, nodeWidth, nodeHeight);
        map.push_back(mapRect);
    }
    for (i=0; i<50; i++)
    {
        QRect mapRect(49*nodeWidth, i*nodeHeight, nodeWidth, nodeHeight);
        map.push_back(mapRect);
    }
    for (i=1; i<49; i++)
    {
        QRect mapRect(i*nodeWidth, 49*nodeHeight, nodeWidth, nodeHeight);
        map.push_back(mapRect);
    }


    /* 初始化按钮，输入框 */
    int midX = this->width()/2;
    int midY = this->height()/2;
    continuePushButton.setGeometry(midX-BUTT_WIDTH/2, midY*3/2, BUTT_WIDTH, BUTT_HEIGT);
    continuePushButton.setParent(this);
    continuePushButton.setText(tr("Got it"));
    continuePushButton.setVisible(false);
    confirmPushButton.setGeometry(midX-BUTT_WIDTH/2, midY*3/2, BUTT_WIDTH, BUTT_HEIGT);
    confirmPushButton.setParent(this);
    confirmPushButton.setText(tr("OK"));
    confirmPushButton.setVisible(false);
    replayPushButton.setGeometry(midX-BUTT_WIDTH/2, midY*3/2, BUTT_WIDTH, BUTT_HEIGT);
    replayPushButton.setParent(this);
    replayPushButton.setText(tr("Try again"));
    replayPushButton.setVisible(false);
    nameLine.setGeometry(midX-50, 250, 100, 20);
    nameLine.setParent(this);
    nameLine.setVisible(false);


    btLabel.setParent(this);
    btLabel.setGeometry(0, 502, 500, 18);
    QFont font;
    font.setBold(true);
    font.setItalic(true);

    btLabel.setFont(font);
    btLabel.setAlignment(Qt::AlignRight);
    btLabel.setText(tr(" Score: [%1] ").arg(0));
    btLabel.setVisible(true);


//    timerFd = startTimer(timeVal);
    /* 启动计时器 */
    t.setParent(this);
    t.setInterval(timeVal);	// 设置时间间隔
    t.start();	// 开始计时
}



Snake::~Snake()
{
    delete ui;
}


/* 重载按键按下事件 */
void Snake::keyPressEvent(QKeyEvent *k)
{
    int keyCode = k->key();	// 获取按键代码
    if (!k->isAutoRepeat())	// 不是长按的情况
    {
        switch(keyCode)
        {
        case Qt::Key_Up:	// 向上
            velocityVector.setX(0);
            velocityVector.setY(-nodeHeight);
            vList.enqueue(velocityVector);	// 进入按键队列
            break;
        case Qt::Key_Down:
            velocityVector.setX(0);
            velocityVector.setY(nodeHeight);
            vList.enqueue(velocityVector);
            break;
        case Qt::Key_Right:
            velocityVector.setX(nodeWidth);
            velocityVector.setY(0);
            vList.enqueue(velocityVector);
            break;
        case Qt::Key_Left:
            velocityVector.setX(-nodeWidth);
            velocityVector.setY(0);
            vList.enqueue(velocityVector);
            break;
        default:
            return;
        }
    }
    /* 长按加速功能，按键方向必须和蛇的前进方向一致 */
    if (k->isAutoRepeat() && flag && (keyCode == currentDir))
    {
        qDebug() << "press: auto repeat";
	t.setInterval(40);	// 重新设置计时器时间间隔
//        timerFd = startTimer(50);
        flag = false;	// flag=false表示进入按键长按状态
    }
    if (vList.length() > 1)
        vList.pop_back();	// 按键队列不能超过1
}

/* 重载按键释放 */
void Snake::keyReleaseEvent(QKeyEvent *k)
{
    if (!k->isAutoRepeat() && !flag)	// 如果不是长按状态--附：Qt下长按事件即不断重复keypress和keyrelease
    {
        if (speedFlag)	// 在加速状态下吃到食物，并且满足速度升级条件时，speedflag为true
        {
            qDebug() << "speed up!";
            if (timeVal > 70)
            {
                timeVal = timeVal - 10;	// 进行速度升级
            }
            count = 0;
            speedFlag = false;
        }
        qDebug() << "release: auto repeat";
//        killTimer(timerFd);
//        timerFd = startTimer(timeVal);
	t.setInterval(timeVal);
        flag = true;
    }
}

/* 游戏失败判断和处理 */
void Snake::isLost()
{
    QList<QRect>::Iterator snakeIt = snakeBody.begin();
    QList<QRect>::Iterator mapIt = map.begin();
    snakeIt++;
    for (; snakeIt<snakeBody.end(); snakeIt++)	// 如果蛇碰到自己
    {
        if (snakeHead == *snakeIt)
        {
	    drawEvent = DIE;
	    t.stop();
            return ;
        }
    }
    for (; mapIt<map.end(); mapIt++)	// 如果蛇碰到墙壁
    {
        if (snakeHead == *mapIt)
        {
	    drawEvent = DIE;
	    t.stop();
            return ;
        }
    }
}

/* 吃食物，得分，长大 */
void Snake::eatFood()
{
    if (snakeHead == food)	// 头碰到食物
    {
        QList<QRect>::Iterator it = snakeBody.end();
        QRect newTail;	// 蛇增加的部分
        QRect oldTail(*(--it));
        QRect node(*(--it));
        newTail.setRect(2*oldTail.x()-node.x(), 2*oldTail.y()-node.y(), nodeWidth, nodeHeight);
        snakeBody.push_back(newTail);
        genFood();
        count++;
        score += 100 + extraPoint;	// 获得基本分数加奖励分数
        if (count >= 5)
        {
            if (flag)
            {
                qDebug() << "speed up!";
                if (timeVal > 70)
                {
                    timeVal = timeVal - 10;
//	                killTimer(timerFd);
//      	        timerFd = startTimer(timeVal);
		    t.setInterval(timeVal);
                }
                count = 0;
            }
            else
            {
                speedFlag = true;
            }
        }

        btLabel.setText(tr(" Score: [%1] ").arg(score));	// 显示分数
        luck = rand() % 100;
    }
}

/* 定时被触发的事件 */
void Snake::qTimerEvent()
{
    QList<QRect>::Iterator it = snakeBody.begin();
    int deltaX = (*it).x() - (*(it+1)).x();	// 蛇速度矢量的x分量
    int deltaY = (*it).y() - (*(it+1)).y();	// 蛇速度矢量的y分量
    QPoint deltaDir(deltaX, deltaY);
    QRect newHead;
    QPoint vVector;	// 存放用户产生的方向
    if (vList.isEmpty())
        vVector = velocityVector;
    else
        vVector = vList.dequeue();

    /* 蛇的移动 */
    if (!(deltaDir.x()*vVector.x()+deltaDir.y()*vVector.y()))	// 满足垂直条件
    {
        newHead.setRect((*it).x()+vVector.x(), (*it).y()+vVector.y(), nodeHeight, nodeWidth);
        setCurrentDir(vVector.x(), vVector.y());
    }
    else
    {
        newHead.setRect((*it).x()+deltaX, (*it).y()+deltaY, nodeHeight, nodeWidth);
        setCurrentDir(deltaX, deltaY);
    }
    snakeBody.pop_back();
    snakeBody.push_front(newHead);
    snakeHead = newHead;
    eatFood();
    isLost();
    update();	// 刷新画面（调paintEvent()）
}

/* 画图事件 */
void Snake::paintEvent(QPaintEvent *)
{
    QRegion region(0, 0, 500, 501);


    QPainter painter(this);
    painter.setClipRegion(region);	// 设置画布大小
    QBrush brush(Qt::Dense6Pattern);	// 设置刷子模式
    painter.setBrush(brush);	// 把brush给painter
    QList<QRect>::Iterator it = snakeBody.begin();
    QList<QRect>::Iterator mapIt = map.begin();

    int fW = 400;
    int fH = 200;
    int midX = this->width()/2;
    int midY = this->height()/2;
    QRect rect(midX-fW/2, midY-fH/2, fW, fH);	// 预设文字占用画布大小
    QRectF rectF(rect);

    switch (drawEvent)
    {
    case PLAY:	// 游戏进行时
    {
        table.setVisible(false);
        replayPushButton.setVisible(false);
        for (; mapIt<map.end();mapIt++)
        {
            painter.drawRect(*mapIt);
        }
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::red);
        painter.setBrush(brush);
        painter.drawEllipse(*(it++));

	/* 画彩色的蛇 */
        QColor color;
        for (; it<snakeBody.end(); it++)
        {
            int r, g, b;
            r = rand() % 256;
            g = rand() % 256;
            b = rand() % 256;
            color.setRgb(r, g, b);
            brush.setColor(color);
            painter.setBrush(brush);
            painter.drawRect(*it);
        }

	/* 根据luck生成不同颜色的食物，对应不同的奖励 */
        if (luck < 80)
        {
            color.setRgb(0, 0, 220);
            extraPoint = 0;
        }
        else if (luck < 95)
        {
            color.setRgb(0, 220, 0);
            extraPoint = 70;
        }
        else
        {
            color.setRgb(220, 0, 0);
            extraPoint = 550;
        }
        brush.setColor(color);
        painter.setBrush(brush);
        painter.drawRect(food);
        break;
    }
    case DIE:	// 死亡
    {
        painter.drawText(rectF, Qt::AlignCenter, tr("Game over!\n\nYou score is: %1 ").arg(score));
        continuePushButton.setVisible(true);
        break;
    }
    case HIGHSCORE:	// 新的高分，用户输入名称界面
    {
        painter.drawText(rectF, Qt::AlignCenter, tr("New high score\n\n Please enter you name"));
        continuePushButton.setVisible(false);
        nameLine.setGeometry(midX-50, 290, 100, 50);
        nameLine.setAlignment(Qt::AlignCenter);
        nameLine.setVisible(true);
        confirmPushButton.setVisible(true);

//        painter.drawText(198, 150, "New high score");
//        painter.drawText(175, 200, "Please enter you name");

        break;
    }
    case DISPHIGH:	// 显示最高分
    {
        continuePushButton.setVisible(false);
        nameLine.setVisible(false);
        confirmPushButton.setVisible(false);

        QSqlTableModel *tModel = new QSqlTableModel(this);
        tModel->setTable("score");	// 选择要显示的表
        if (tModel->select())
        {
	    table.setParent(this);
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
            table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            table.verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
	    table.horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	    table.verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
            table.setSortingEnabled(true);
            table.setModel(tModel);	// 将tModel载入table显示
            table.hideColumn(0);
            table.sortByColumn(2);
            table.setGeometry(this->width()/2-200, 40, 400, 300);
            table.setVisible(true);
        }
        replayPushButton.setVisible(true);	// 显示重玩按钮


        break;
    }
    }
}

/* 读取高分 */
void Snake::readHighScore()
{
    qDebug() << "readHighScore";

    if (myDB.isHighScore(score) == SNAKE_DB_IS_NEWHIGH)
        drawEvent = HIGHSCORE;
    else
        drawEvent = DISPHIGH;
    update();
}

/* 保存高分 */
void Snake::saveRecordAndDispHigh()
{
    if (!nameLine.text().length())
    {
        QMessageBox::warning(this, "ERROR", "Name can not be empty");
        return ;
    }
    qDebug() << "saveRecordAndDispHigh";

    if (SNAKE_DB_ERR_SUCCESS == myDB.saveRecord(nameLine.text().toLatin1().data(), score))
    {
        drawEvent = DISPHIGH;
        update();
    }
}

/* 重玩 */
void Snake::replay()
{
    initialize();
    this->setFocus();
    update();
}

/* 设置当前方向，用于长按建时判断是否在前进方向上 */
void Snake::setCurrentDir(int x, int y)
{
    if (x>0 && !y)
        currentDir = Qt::Key_Right;
    else if (!x && y>0)
        currentDir = Qt::Key_Down;
    else if (x<0 && !y)
        currentDir = Qt::Key_Left;
    else if (!x && y<0)
        currentDir = Qt::Key_Up;
}

void Snake::genFood()
{

    srand(time(NULL));
    int x = 0;
    int y = 0;
    while (1)
    {
        QList<QRect>::Iterator it = snakeBody.begin();
        x = nodeWidth*(random()%47+1);
        y = nodeHeight*(random()%47+1);
        int coinFlag = true;
        for (; it!=snakeBody.end(); it++)
        {
            if ((*it).x()==x && (*it).y()==y)
                coinFlag = false;
        }
        if (coinFlag)
            break;
    }
    food.setX(x);
    food.setY(y);
    food.setHeight(nodeHeight);
    food.setWidth(nodeWidth);
}
