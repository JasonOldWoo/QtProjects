#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QTcpSocket>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class SignUp : public QDialog
{
    Q_OBJECT
    
public:
    explicit SignUp(QWidget *parent = 0);
    ~SignUp();
    
private slots:
    void on_pushButton_clicked();

    void slotHostFound();
    void slotConnected();
    void slotDisconnected();
    void slotReadData();
    void slotError();

signals:
    void signalPushButtonClicked();

private:
    QPushButton *pushButton;
};

#endif // SIGNUP_H
