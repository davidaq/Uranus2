#include "mainwindow.h"
#include "configdlg.h"
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
    benchModel->setReadOnly(false);
    benchModel->setNameFilterDisables(false);

    QVariant var=Cfg::get("benchFilter");
    if(var.isValid())
    {
        ui->benchFileFilter->setText(var.toString());
    }

    ui->fileView->setModel(benchModel);
    ui->fileView->hideColumn(1);
    ui->fileView->hideColumn(2);
    ui->fileView->hideColumn(3);

    connect(ui->actionRestart_Console,SIGNAL(triggered()),ui->consoleWidget,SLOT(shellRestart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

Console* MainWindow::console()
{
    return ui->consoleWidget;
}

QTreeView* MainWindow::fileView()
{
    return ui->fileView;
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
    Cfg::set("benchFilter",filter);
}

void MainWindow::on_actionConfiguration_triggered()
{
    Cfg::pauseAlarm(true);
    ConfigDlg dlg;
    dlg.exec();
    Cfg::pauseAlarm(false);
}

