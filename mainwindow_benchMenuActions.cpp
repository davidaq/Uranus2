#include "mainwindow.h"
#include "console.h"
#include "ualgorithmeditor.h"
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

void MainWindow::on_fileView_doubleClicked(const QModelIndex &index)
{
    fileView()->setCurrentIndex(index);
    if(index.isValid())
    {
        benchMenuTarget = index;
        benchMenu_open();
    }
}

void MainWindow::on_fileView_customContextMenuRequested(const QPoint &pos)
{
    static QMenu menu(this);
    QModelIndex item=fileView()->indexAt(pos);
    fileView()->setCurrentIndex(item);

    benchMenuTarget=item;
    menu.clear();

    menu.addAction( QIcon(":/images/folder-new.png") , "Make Directory" , this , SLOT(benchMenu_mkdir()) );
    menu.addAction( QIcon(":/images/document-new.png") , "Create empty file" , this , SLOT(benchMenu_newFile()) );

    if(item.isValid())
    {
        menu.addSeparator();
        menu.addAction( "Rename",this,SLOT(benchMenu_rename()) );
        if(benchModel->isDir(item))
        {
            menu.addAction( "Set as base directory",this,SLOT(benchMenu_setbase()) );
        }else
        {
            menu.setDefaultAction(menu.addAction( "Open as algorithm",this,SLOT(benchMenu_open()) ));
            menu.addAction( "Run in console",this,SLOT(benchMenu_runInConsole()) );
        }
        menu.addSeparator();
        menu.addAction( QIcon(":/images/delete.png"),"Delete",this,SLOT(benchMenu_delete()) );
    }
    menu.move(fileView()->mapToGlobal(pos));
    menu.show();
}

void MainWindow::benchMenu_mkdir()
{
    QModelIndex item;
    if(!benchMenuTarget.isValid())
    {
        item=fileView()->rootIndex();
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
        int newI=2;
        while(dir.exists(name+QString().sprintf(" %d",newI)))
            newI++;
        name+=QString().sprintf(" %d",newI);
    }
    item=benchModel->mkdir(item,name);
    fileView()->setCurrentIndex(item);
    fileView()->edit(item);
}

void MainWindow::benchMenu_newFile()
{

    QModelIndex item;
    if(!benchMenuTarget.isValid())
    {
        item=fileView()->rootIndex();
    }else if(benchModel->isDir(benchMenuTarget))
    {
        item=benchMenuTarget;
    }else
    {
        item=benchMenuTarget.parent();
    }
    QDir dir(benchModel->filePath(item));
    QString name="Empty File";
    if(dir.exists(name))
    {
        int newI=2;
        while(dir.exists(name+QString().sprintf(" %d",newI)))
            newI++;
        name+=QString().sprintf(" %d",newI);
    }

    name=dir.absoluteFilePath(name);
    QFile fp(name);
    if(fp.open(QFile::ReadWrite))
        fp.close();
    item=benchModel->index(name);
    fileView()->setCurrentIndex(item);
    fileView()->edit(item);
}

void MainWindow::benchMenu_rename()
{
    fileView()->edit(benchMenuTarget);
}

void MainWindow::benchMenu_delete()
{
    QString name=benchModel->fileName(benchMenuTarget);
    if(QMessageBox::Yes==QMessageBox::warning(
        this,"Delete confirm","Are you sure you want to delete:\n  "+name+"\nTHIS CAN NOT BE UNDONE!",
        QMessageBox::Yes|QMessageBox::No))
    {
        benchModel->remove(benchMenuTarget);
    }
}

void MainWindow::benchMenu_open()
{
    QString path=benchModel->filePath(benchMenuTarget);
    QFile fp(path);
    if(fp.open(QFile::ReadOnly)){
        QString head = QString::fromUtf8(fp.readLine()).trimmed();
        fp.close();
        if(head==FILE_HEAD){
            on_actionOpen_triggered(path);
        }
    }
}

void MainWindow::benchMenu_runInConsole()
{
    QString path=benchModel->filePath(benchMenuTarget);
    QDir base=QDir(benchModel->rootPath());
    QFileInfo info(path);
    path=base.relativeFilePath(path);
    QString ext=info.suffix();
    QString cmd;
    if(ext=="py"||ext=="pyt")
    {
        cmd="execfile('"+path+"')";
    }else
    {
        cmd=path;
    }
    console()->prepareCmd(cmd);
}

void MainWindow::benchMenu_setbase()
{
    console()->cd(benchModel->filePath(benchMenuTarget));
}

