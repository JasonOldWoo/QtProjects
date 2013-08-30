#include "filemanage.h"
#include "ui_filemanage.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QByteArray>
#include <QDebug>
#include <QStandardItem>
#include <QGraphicsColorizeEffect>
#include <QGraphicsEffect>

FileManage::FileManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileManage)
{
    ui->setupUi(this);
    ui->contentText->setReadOnly(true);
    ui->editButton->setEnabled(false);
    ui->closeFileButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->overwriteButton->setEnabled(false);
    ui->openButton->setEnabled(false);
    currentPath.clear();

    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browseFileToOpen()));
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editText()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(browsePathToSave()));
    connect(ui->overwriteButton, SIGNAL(clicked()), this, SLOT(overwriteFile()));
    connect(ui->closeFileButton, SIGNAL(clicked()), this, SLOT(closeFile()));
    connect(ui->pathLine, SIGNAL(textChanged(QString)), this, SLOT(openButtonEnable()));
    connect(ui->contentText, SIGNAL(textChanged()), this, SLOT(overwriteEnable()));
}

FileManage::~FileManage()
{
    delete ui;
}

void FileManage::browseFileToOpen()
{
    if (ui->overwriteButton->isEnabled())
    {
        if (QMessageBox::Cancel == QMessageBox::information(this, tr("warning"),
                                                            tr("file has been modified, press OK to ignore it"),
                                                            QMessageBox::Ok | QMessageBox::Cancel))
        {
            return ;
        }
    }
    QDir defaultPath;
    defaultPath.setPath(currentPath);
    if (!defaultPath.isReadable())
    {
        defaultPath = QDir::homePath();
    }
    QString path = QFileDialog::getOpenFileName(this, tr("Choose File"),
                                                defaultPath.absolutePath(),
                                                tr("C/C++ File(*.cpp *.c *.h);;All File(*)"));
    if (!path.isEmpty())
    {
        ui->pathLine->setText(path);
    }

    QFile f(path);
    if (path.isEmpty())
    {
        return ;
    }
    if (!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,tr("Can Not Open File"), tr("file do not exist or have no permission"));
        ui->pathLine->setText(currentPath);
        return ;
    }
    QByteArray data = f.readAll();
    if (data.length() > 1024*1024)
    {
        QMessageBox::warning(this, tr("Can Not Open File"), tr("file is too large to load"));
        ui->pathLine->setText(currentPath);
        return;
    }

    ui->openButton->setEnabled(false);
    currentPath = path;
    ui->contentText->setText(data);
    ui->editButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->overwriteButton->setEnabled(false);
    ui->closeFileButton->setEnabled(true);
    f.close();

}

void FileManage::browsePathToSave()
{
    QDir dir = ui->pathLine->text();
    if (!dir.isReadable())
    {
        dir = QDir::home();
    }
    QFileDialog fDialog;

    QString savePath = fDialog.getSaveFileName(this, tr("Save File"),
                                                dir.absolutePath());
    if (!savePath.isEmpty())
    {
        QFile f(savePath);
        if (!f.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this, tr("Error"),
                                 tr("this file may be in using now"));
            f.close();
            return ;

        }
        QByteArray data;
        data.append(ui->contentText->toPlainText());
        if (-1 == f.write(data))
        {
            QMessageBox::warning(this, tr("Error"),
                                 tr("fail to overwrite"));
            f.close();
            return ;
        }
        QMessageBox::information(this, tr("Save Success"),
                                 tr("file has been saved"));
        f.close();
    }

}
void FileManage::openFile()
{
    if (ui->overwriteButton->isEnabled())
    {
        if (QMessageBox::Cancel == QMessageBox::information(this, tr("warning"),
                                                            tr("file has been modified, press OK to ignore it"),
                                                            QMessageBox::Ok | QMessageBox::Cancel))
        {
            return ;
        }
    }
    QFile f(ui->pathLine->text());
    if (ui->pathLine->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Can Not Open Filconnecte"), tr("path could not be empty"));
        ui->pathLine->setText(currentPath);
        return ;
    }
    if (!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,tr("Can Not Open File"), tr("file do not exist or have no permission"));
        ui->openButton->setEnabled(false);
        ui->pathLine->setText(currentPath);
        return ;
    }
    QByteArray data = f.readAll();
    if (data.length() > 1024*1024)
    {
        QMessageBox::warning(this, tr("Can Not Open File"), tr("file is too large to load"));
        ui->pathLine->setText(currentPath);
        return;
    }

    ui->openButton->setEnabled(false);
    currentPath = ui->pathLine->text();
    ui->contentText->setText(data);
    ui->editButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->overwriteButton->setEnabled(false);
    ui->closeFileButton->setEnabled(true);
    f.close();
}

void FileManage::editText()
{
    ui->contentText->setReadOnly(false);
    ui->contentText->setFocusPolicy(Qt::StrongFocus);
    ui->editButton->setEnabled(false);
    ui->overwriteButton->setEnabled(false);
    //ui->saveButton->setEnabled(true);
}

void FileManage::overwriteFile()
{
    QFile f(currentPath);
    if (!f.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,tr("Can Not Overwrite"), tr("this file may be in using now or read only"));
        return ;
    }
    QByteArray data;
    data.append(ui->contentText->toPlainText());
    if (-1 == f.write(data))
    {
        QMessageBox::warning(this,tr("Can Not Overwrite"), tr("fail to overwrite, please try again"));
        return ;
    }
    ui->overwriteButton->setEnabled(false);

}

void FileManage::closeFile()
{
    if (ui->overwriteButton->isEnabled())
    {
        QMessageBox qMBox;
        if (qMBox.information(this, tr("warning"),
                          tr("file has been modified, press OK to close it without saving change"),
                          QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
        {
            return;
        }

    }
    ui->pathLine->clear();
    ui->contentText->clear();
    ui->editButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->overwriteButton->setEnabled(false);
    ui->closeFileButton->setEnabled(false);
    ui->openButton->setEnabled(false);
    currentPath.clear();

}

void FileManage::openButtonEnable()
{
    ui->openButton->setEnabled(true);
}

void FileManage::overwriteEnable()
{
    if (ui->saveButton->isEnabled())
    ui->overwriteButton->setEnabled(true);
}
