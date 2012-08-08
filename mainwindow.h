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
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QTreeView* fileView;
    QFileSystemModel *benchModel;
    QModelIndex benchMenuTarget;

private slots:
    void cwdChanged(QString);
    void on_setCwdBtn_clicked();
    void on_action_BenchRename_triggered();
    void on_benchView_itemChanged(QTreeWidgetItem *item, int column);
    void on_fileView_customContextMenuRequested(const QPoint &pos);
    void on_benchFileFilter_textChanged(const QString &arg1);

    // bench menu actions
    void benchMenu_mkdir();
    void benchMenu_newFile();
    void benchMenu_rename();
};

#endif // MAINWINDOW_H
