#include "filemanage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileManage w;
    w.show();

    return a.exec();
}
