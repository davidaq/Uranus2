#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->consoleWidget,SIGNAL(cwdChanged(QString)),SLOT(cwdChanged(QString)));

    QTreeWidgetItem* item=new QTreeWidgetItem(QStringList()<<"Bookmarks");
    item->setIcon(0,QIcon(":/images/folder-bookmarks.png"));
    ui->benchView->addTopLevelItem(item);
    item->setExpanded(true);
    item=new QTreeWidgetItem(QStringList()<<"Working directory");
    item->setIcon(0,QIcon(":/images/folder-home.png"));
    ui->benchView->addTopLevelItem(item);
    item->setExpanded(true);
    benchCwd=item;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cwdChanged(QString path)
{
    QString name=QDir(path).dirName();
    if(name.length()>27)
        name=name.left(25)+"...";
    ui->setCwdBtn->setText(name);
    ui->setCwdBtn->setToolTip(path);
    refreshBench();
}

void MainWindow::on_setCwdBtn_clicked()
{
    QString path=QFileDialog::getExistingDirectory(this,"Set the bench directory",ui->consoleWidget->getCwd());
    if(!path.isEmpty())
    {
        ui->consoleWidget->cd(path);
    }
}

void MainWindow::refreshBench()
{
    foreach(QTreeWidgetItem* item,benchCwd->takeChildren())
    {
        delete item;
    }
    benchCwd->setToolTip(0,ui->consoleWidget->getCwd());
    benchOpenDir(benchCwd);
}

void MainWindow::benchOpenDir(QTreeWidgetItem *parent)
{
    QString base=(QString)parent->toolTip(0);
    QDir dir(base);
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot|QDir::Readable|QDir::NoSymLinks);
    dir.setSorting(QDir::Name|QDir::DirsFirst);
    QFileInfoList list=dir.entryInfoList();
    foreach(QFileInfo info,list){
        QTreeWidgetItem* item=new QTreeWidgetItem(QStringList()<<info.fileName());
        QString icon;
        parent->addChild(item);
        item->setFlags(item->flags()|Qt::ItemIsEditable);
        item->setToolTip(0,info.filePath());
        if(info.isDir())
        {
            icon=":/images/folder.png";
            benchOpenDir(item);
            item->setData(0,5,true);
        }else
        {
            icon=":/images/document.png";
            item->setData(0,5,false);
        }
        item->setIcon(0,QIcon(icon));
    }
}

void MainWindow::on_benchView_customContextMenuRequested(const QPoint &pos)
{
    static QMenu menu;
    menu.clear();
    menuTarget=ui->benchView->itemAt(pos);

    menu.addAction(ui->action_BenchCreateEmptyDocument);
    menu.addAction(ui->action_BenchMakeFolder);

    if(menuTarget!=0&&!menuTarget->data(0,5).isNull()){
        ui->benchView->setCurrentItem(menuTarget);
        menu.addSeparator();
        if(!menuTarget->data(0,5).toBool())
        {
            menu.addAction(ui->action_BenchEdit);
            menu.addAction(ui->action_BenchDuplicate);
        }
        menu.addAction(ui->action_BenchRename);
        menu.addSeparator();
        menu.addAction(ui->action_BenchDelete);
    }
    menu.move(ui->benchView->mapToGlobal(pos));
    menu.show();
}

// Bench Menu Actions
void MainWindow::on_action_BenchRename_triggered()
{
    if(menuTarget==0)
        return;
    ui->benchView->editItem(menuTarget);
}

void MainWindow::on_benchView_itemChanged(QTreeWidgetItem *item, int)
{
    if(item->data(0,5).isNull())
        return;
    QString path=item->toolTip(0);
    QDir dir(path);
    QString oldName=dir.dirName(),newName=item->text(0);
    dir.cd("..");
    if(!dir.exists(newName))
    {
        dir.rename(oldName,newName);
    }
}
