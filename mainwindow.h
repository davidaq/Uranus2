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
private:
    Ui::MainWindow *ui;
    QFileSystemModel *benchModel;
    QModelIndex benchMenuTarget;

private slots:
    void cwdChanged(QString);
    void on_setCwdBtn_clicked();
    void on_fileView_customContextMenuRequested(const QPoint &pos);
    void on_benchFileFilter_textChanged(const QString &arg1);

    // bench menu actions
    void benchMenu_mkdir();
    void benchMenu_newFile();
    void benchMenu_rename();
    void benchMenu_delete();
    void benchMenu_runInConsole();
    void benchMenu_setbase();
    void on_actionConfiguration_triggered();
};

#endif // MAINWINDOW_H
