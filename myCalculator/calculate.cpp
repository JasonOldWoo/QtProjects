#include <QStack>
#include <QQueue>
#include <QString>
#include <QDebug>
#include "calculate.h"

Calc::Calc(QString &expr):expr(expr){}

QString Calc::getRes()
{
    invPoland();
    QString strZero = "/";
    QString strNine = ":";
    QString mathError = "MATH ERROR";
    QStack<QString>s;
    while (!q.empty())
    {
        QString tmp = q.dequeue();
        if ("+"==tmp || "-"==tmp || "*"==tmp || "/"==tmp)
        {
            QString num2 = s.pop();
            QString num1 = s.pop();
            QString res;
            qDebug() << "num1: " << num1;
            qDebug() << "num2: " << num2;
            qDebug() << "opt: " << tmp;
            if ("+"==tmp)
            {
                double r = num1.toDouble()+num2.toDouble();
                res = QString::number(r, 'g', 15);
//                qDebug() << "r" << r;
//                qDebug() << "res" << res;
            }
            else if ("-"==tmp)
            {
                double r = num1.toDouble()-num2.toDouble();
                res = QString::number(r, 'g', 15);
            }
            else if ("*"==tmp)
            {
                double r = num1.toDouble()*num2.toDouble();
                res = QString::number(r, 'g', 15);
            }
            else if ("/"==tmp)
            {
                //qDebug() << num2.toDouble();
                if (num2.toDouble()<1e-100 && num2.toDouble()>-1e-100)
                {
                    //qDebug() << "mathError";
                    return mathError;
                }
                double r = num1.toDouble()/num2.toDouble();
                res = QString::number(r, 'g', 15);
            }
            s.push(res);
        }
        else
        {
            s.push(tmp);
        }
    }
    return s.pop();
}

void Calc::invPoland()
{
    QString::Iterator it = expr.begin();
    QString strZero = "/";
    QString strNine = ":";
    QString num = "initialize";
    num.clear();
    if ("-" == *it)
    {
        num.append("-");
        it++;
    }
    QStack<QString>s;
    for (; it<expr.end(); it++)
    {
        while ((*it>strZero && *it<strNine) || "."==(*it) || "e"==*it)
        {
            if ("e" == *it)
            {
                num.append(*it);
                it++;
                num.append(*it);
                it++;
            }
            else
            {
                num.append(*it);
                it++;
            }
        }
        if (!num.isEmpty())
        {
            q.enqueue(num);
            qDebug() << "num: " << num;
        }
        num.clear();

        if ("+"==*it || "-"==*it || "*"==*it || "/"==*it)
        {
//            qDebug() << "stack is empty?" << s.isEmpty();
//            qDebug() << "new opt: " << *it;
            while (!s.isEmpty() && !(("*"==*it || "/"==*it) && ("+"==s.top() || "-"==s.top())))
            {
                QString opt = s.top();
                if ("="!=opt && "-"!=opt && "*"!=opt && "/"!=opt)
                {
                    break;
                }
                QString tmpOpt = s.pop();
                q.enqueue(tmpOpt);
                qDebug() << "en opt: " << tmpOpt;
            }
            s.push(*it);
            //qDebug() << "pu opt: " << *it;
        }

        if ("(" == *it)
        {
            s.push(*it);
        }
        if (")" == *it)
        {
            while ("(" != s.top())
            {
                QString tmp = s.pop();
                q.enqueue(tmp);
                qDebug() << tmp;
            }
            s.pop();
        }

    }
    while (!s.isEmpty())
    {
        QString tmp = s.pop();
        q.enqueue(tmp);
        qDebug() << tmp;
    }
}
