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

    benchModel=new QFileSystemModel;
    benchModel->setNameFilterDisables(false);

    ui->fileView->setModel(benchModel);
    ui->fileView->hideColumn(1);
    ui->fileView->hideColumn(2);
    ui->fileView->hideColumn(3);

    fileView=ui->fileView;
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
    benchModel->setRootPath(path);
    ui->fileView->setRootIndex(benchModel->index(path));
}

void MainWindow::on_setCwdBtn_clicked()
{
    QString path=QFileDialog::getExistingDirectory(this,"Set the bench directory",ui->consoleWidget->getCwd());
    if(!path.isEmpty())
    {
        ui->consoleWidget->cd(path);
    }
}

void MainWindow::on_benchFileFilter_textChanged(const QString &filter)
{
    benchModel->setNameFilters((filter.isEmpty()?"*":filter).split(';'));
}

void MainWindow::on_fileView_customContextMenuRequested(const QPoint &pos)
{
    static QMenu menu(this);
    QModelIndex item=ui->fileView->currentIndex();
    benchMenuTarget=item;
    menu.clear();

    menu.addAction( QIcon(":/images/folder-new.png") , "Make Directory" , this , SLOT(benchMenu_mkdir()) );
    if(benchModel->isDir(item))
    {
    }
    menu.move(ui->fileView->mapToGlobal(pos));
    menu.show();
}

// Bench Menu Actions
void MainWindow::on_action_BenchRename_triggered()
{
}

void MainWindow::on_benchView_itemChanged(QTreeWidgetItem *item, int)
{
}
