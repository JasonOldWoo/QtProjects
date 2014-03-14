#include "lcserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LCServer w;
    w.show();
    
    return a.exec();
}
