#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cfg.h"

#include <QMainWindow>

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
    void refreshBench();
    void benchOpenDir(QTreeWidgetItem*);
    QTreeWidgetItem *benchCwd,*menuTarget;

private slots:
    void cwdChanged(QString);
    void on_setCwdBtn_clicked();
    void on_benchView_customContextMenuRequested(const QPoint &pos);
    void on_action_BenchRename_triggered();
    void on_benchView_itemChanged(QTreeWidgetItem *item, int column);
};

#endif // MAINWINDOW_H
