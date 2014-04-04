#ifndef LANCHATCLIENT_H
#define LANCHATCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QString>
#include "LCDBInfo.h"
#include "signup.h"
#include "LCPdu.h"

namespace Ui {
class LanChatClient;
}

class LanChatClient : public QWidget
{
    Q_OBJECT
    
public:
    explicit LanChatClient(QWidget *parent = 0);
    ~LanChatClient();
    
private slots:
    void on_expandPushButton_clicked();
    void on_loginPushButton_clicked();
    void on_msgLineEdit_returnPressed();

    void slotHostFound();
    void slotConnected();
    void slotDisconnected();
    void slotReadData();
    void slotError();
    void slotSendInfo();
    void slotReadMsg();

    void on_signUpPushButton_clicked();

signals:
    void signalAuthenticated();

private:
    Ui::LanChatClient *ui;
    bool isShow;
    QTcpSocket *socket;
    QUdpSocket *chatSocket;
    QString username;
    QString password;
    SignUp *signUp;
    quint32 m_userIndex;
    UserInfoList friendList;

private:
    void dillAuthInfo(const char *inbuf, uint len);
    void dillFriendList(const char *inbuf, uint len);
    void dillUserLogin(const char *inbuf, uint len);
    void dillUserLogout(const char *inbuf, uint len);
};

#endif // LANCHATCLIENT_H
