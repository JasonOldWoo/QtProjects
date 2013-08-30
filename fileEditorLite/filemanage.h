#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class FileManage;
}

class FileManage : public QWidget
{
    Q_OBJECT
    
public:
    explicit FileManage(QWidget *parent = 0);
    ~FileManage();
    
private:
    Ui::FileManage *ui;
    QString currentPath;


public slots:
    void browseFileToOpen();
    void browsePathToSave();
    void openFile();
    void editText();
    void overwriteFile();
    void closeFile();
    void openButtonEnable();
    void overwriteEnable();
};

#endif // FILEMANAGE_H
