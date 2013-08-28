#include <QDebug>
#include "calculator.h"
#include "calculate.h"
#include "ui_calculator.h"

void Calculator::calc()
{
    QString expr = ui->lineEdit->text();
    if(rightExpr(expr))
    {
        qDebug() << expr;
        Calc c(expr);
        ui->lineEdit->setText(c.getRes());
    }
    else
    {
        qDebug() << "error input";
    }
    return ;
}

bool Calculator::rightExpr(QString &expr)
{
    QString newExpr = "";
    QString::Iterator it = expr.begin();
    QString::Iterator itLast = expr.end();
    QString last = *(--itLast);
    int bras = 0;
    int kets = 0;
    for (; it<expr.end(); it++)
    {
        if ("(" == *it)
        {
            bras++;
        }
        if (")" == *it)
        {
            kets++;
        }
    }
    if (kets != bras)
    {
        return false;
    }
    if ("+"==last || "-"==last || "*"==last || "/"==last || "("==last)
    {
        return false;
    }
    it = expr.begin();
    QString strZero = "/";
    QString strNine = ":";
    for (; it<expr.end(); it++)
    {
        newExpr.append(*it);
        if (it != itLast-1 && *it>strZero && *it<strNine)
        {
            if ("(" == *(it+1))
            {
                newExpr.append("*");
            }
        }
        if (it != itLast && ")" == *it)
        {
            if ("("==*(it+1) || (*(it+1)>strZero&&*(it+1)<strNine))
            {
                newExpr.append("*");
            }
        }
    }
    expr = newExpr;
    return true;
}
