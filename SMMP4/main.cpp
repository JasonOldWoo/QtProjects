#include "mysql_widget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mysql_Widget w;

    QTextCodec *codec=QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForTr(codec);

    w.show();
    return a.exec();
}
