#include "lanqqserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LanQQServer w;
    w.show();
    
    return a.exec();
}
