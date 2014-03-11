#ifndef MYSQL_WIDGET_H
#define MYSQL_WIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QString>
#include <QHostAddress>
#include <QTcpSocket>
#include <QDir>
#include <QDateTime>
#include <QListWidget>
#include <QProcess>

namespace Ui {
class mysql_Widget;
}

class mysql_Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit mysql_Widget(QWidget *parent = 0);
    ~mysql_Widget();
    QString map_lat;
    QString map_lng;
    QString date;
    
private slots:
    void on_connect_pushButton_clicked();

    void on_connect_net_pushButton_clicked();

    void on_webview_pushButton_clicked();

private:
    Ui::mysql_Widget *ui;
    QSqlTableModel *model;

    QString userName;
    int port;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;
    bool status;
    bool isGotFrameHead;
    QTimer *timer;
    QTimer *refresh_time;
    QByteArray *frameDataArray;
    int frameLength;
    uchar *pp;
    char *p;
    QImage *frame;
    QString saveFileName;

    void sendRequest();
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    void refresh();
    void timerEvent(QTimerEvent *);

public slots:
    void dataReceived();
    void slotDisconnected();
    void slotConnected();
    void slotStatusChange(QAbstractSocket::SocketState state);

private slots:

    void on_pushButton_2_clicked();
    void on_enterPushButton_clicked();
    void on_refresh_pushButton_clicked();
    void on_stu_refresh_pushButton_clicked();
    void on_pushButton_4_clicked();
    void slot_refresh_time();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // MYSQL_WIDGET_H
