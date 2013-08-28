#ifndef POSTEXPR_H
#define POSTEXPR_H


#include <QQueue>
#include <QString>

class Calc
{
public:
    Calc(QString &);
    QString getRes();

private:
    void invPoland();

private:
    QString expr;
    QQueue<QString>q;
};

#endif // POSTEXPR_H
