#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cfg.h"

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTreeView>

namespace Ui {
class MainWindow;
}
class QTreeWidgetItem;
class Console;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Console* console();
    QTreeView* fileView();
    QString getCwd();
private:
    Ui::MainWindow *ui;
    QFileSystemModel *benchModel;
    QModelIndex benchMenuTarget;
    QList<QString> history,historyBack;
    QString benchCurrentPath;
    QTimer tick;

private slots:
    void cwdChanged(QString);
    void on_setCwdBtn_clicked();
    void on_fileView_customContextMenuRequested(const QPoint &pos);
    void on_benchFileFilter_textChanged(const QString &arg1);
    void updateTools();

    // bench menu actions
    void benchMenu_mkdir();
    void benchMenu_newFile();
    void benchMenu_rename();
    void benchMenu_delete();
    void benchMenu_runInConsole();
    void benchMenu_setbase();
    void benchMenu_open();
    void on_actionConfiguration_triggered();
    void on_benchToolUp_clicked();
    void on_benchToolVisible_clicked(bool);
    void on_benchToolMkdir_clicked();
    void on_benchToolBack_clicked();
    void on_benchToolNext_clicked();
    void on_actionNew_Algorithm_triggered();
    void on_actionCasacaded_Display_triggered();
    void on_actionTiled_Display_triggered();
    void on_actionSave_triggered();
    void on_actionSave_All_triggered();
    void on_actionOpen_triggered(QString path="");
    void on_fileView_doubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
