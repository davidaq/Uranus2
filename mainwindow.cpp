#include "mainwindow.h"
#include "configdlg.h"
#include "ualgorithmeditor.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QMdiSubWindow>
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

    ui->benchToolBack->setEnabled(false);
    ui->benchToolNext->setEnabled(false);

    tick.setInterval(500);
    tick.start();
    connect(&tick,SIGNAL(timeout()),SLOT(updateTools()));
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

    if(!benchCurrentPath.isEmpty()&&benchCurrentPath!=path)
    {
        historyBack.clear();
        ui->benchToolNext->setEnabled(false);
        if(!history.isEmpty())
            ui->benchToolBack->setEnabled(true);
        history.append(benchCurrentPath);
        if(history.count()>50)
            history.pop_front();
    }
    benchCurrentPath=path;
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


void MainWindow::on_benchToolUp_clicked()
{
    ui->consoleWidget->cd("..");
}

void MainWindow::on_benchToolVisible_clicked(bool down)
{
    if(down)
    {
        benchModel->setFilter(benchModel->filter()|QDir::Hidden);
    }else
    {
        benchModel->setFilter(~((~benchModel->filter())|QDir::Hidden));
    }
}

void MainWindow::on_benchToolMkdir_clicked()
{
    benchMenuTarget=QModelIndex();
    benchMenu_mkdir();
}

void MainWindow::on_benchToolBack_clicked()
{
    historyBack.append(benchCurrentPath);
    QString path=history.last();
    history.pop_back();
    ui->consoleWidget->cd(path);
    if(history.isEmpty())
        ui->benchToolBack->setEnabled(false);
    ui->benchToolNext->setEnabled(true);
    benchCurrentPath=path;
}

void MainWindow::on_benchToolNext_clicked()
{
    history.append(benchCurrentPath);
    QString path=historyBack.last();
    historyBack.pop_back();
    ui->consoleWidget->cd(path);
    if(historyBack.isEmpty())
        ui->benchToolNext->setEnabled(false);
    ui->benchToolBack->setEnabled(true);
    benchCurrentPath=path;
}

void MainWindow::on_actionNew_Algorithm_triggered()
{
    UAlgorithmEditor* editor=new UAlgorithmEditor(this,this);
    QMdiSubWindow* sub=ui->mdiArea->addSubWindow(editor);
    sub->showMaximized();
}

void MainWindow::on_actionCasacaded_Display_triggered()
{
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_actionTiled_Display_triggered()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_actionSave_triggered()
{
    QMdiSubWindow *sub = ui->mdiArea->currentSubWindow();
    UDocument* doc = dynamic_cast<UDocument*>(sub->widget());
    if(doc!=0){
        if(doc->modified())
            doc->save();
    }
}

void MainWindow::on_actionSave_All_triggered()
{
    foreach(QMdiSubWindow *sub,ui->mdiArea->subWindowList()){
        UDocument* doc = dynamic_cast<UDocument*>(sub->widget());
        if(doc!=0){
            if(doc->modified())
                doc->save();
        }
    }
}

void MainWindow::updateTools()
{
    bool hasModified = false,curModified = false;
    foreach(QMdiSubWindow *sub,ui->mdiArea->subWindowList()){
        UDocument* doc = dynamic_cast<UDocument*>(sub->widget());
        if(doc!=0){
            if(doc->modified()){
                hasModified = true;
                if(sub==ui->mdiArea->currentSubWindow())
                {
                    curModified = true;
                    break;
                }
            }
        }
    }
    ui->actionSave_All->setEnabled(hasModified);
    ui->actionSave->setEnabled(curModified);
}

QString MainWindow::getCwd()
{
    return benchCurrentPath;
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,"Open a file");
    if(path.isEmpty())
        return;
    if(QFileInfo(path).suffix()=="urw"){

    }else{
        on_actionNew_Algorithm_triggered();
        UAlgorithmEditor* editor = dynamic_cast<UAlgorithmEditor*>(ui->mdiArea->currentSubWindow()->widget());
        if(editor!=0)
            editor->open(path);
    }
}
