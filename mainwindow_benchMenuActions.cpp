#include "mainwindow.h"
#include <QInputDialog>
#include <QDebug>

void MainWindow::benchMenu_mkdir()
{
    QModelIndex item;
    if(!benchMenuTarget.isValid())
    {
        item=fileView->rootIndex();
    }else if(benchModel->isDir(benchMenuTarget))
    {
        item=benchMenuTarget;
    }else
    {
        item=benchMenuTarget.parent();
    }
    QDir dir(benchModel->filePath(item));
    QString name="New Folder";
    if(dir.exists(name))
    {
        int newI=1;
        while(dir.exists(name+QString().sprintf(" %d",newI)))
            newI++;
        name+=QString().sprintf(" %d",newI);
    }
    item=benchModel->mkdir(item,name);
    fileView->setCurrentIndex(item);
    fileView->edit(item);
}

void MainWindow::benchMenu_newFile()
{
    QDir dir;
    if(!benchMenuTarget.isValid())
    {
        dir=benchModel->rootDirectory();
    }else if(benchModel->isDir(benchMenuTarget))
    {
        dir=QDir(benchModel->filePath(benchMenuTarget));
    }else
    {
        dir=QDir(benchModel->filePath(benchMenuTarget.parent()));
    }
    QString name;
    bool ok;
    do{
        name=QInputDialog::getText(this,"Create a empty file","New file name",QLineEdit::Normal,"",&ok);
    }while(ok&&!name.isEmpty()&&dir.exists(name));
    if(ok&&!name.isEmpty())
    {
        QFile fp(dir.absoluteFilePath(name));
        if(fp.open(QFile::ReadWrite))
            fp.close();
    }
}

void MainWindow::benchMenu_rename()
{
    fileView->edit(benchMenuTarget);
}
