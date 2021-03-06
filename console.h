#ifndef Console_H
#define Console_H

#include "cfg.h"

#include <QWidget>
#include <QResizeEvent>
#include <QProcess>
#include <QAction>

namespace Ui {
class Console;
}

class Console : public QWidget
{
    Q_OBJECT
    
public:
    explicit Console(QWidget *parent = 0);
    ~Console();
    void cd(QString path);
    QString getCwd();
    void prepareCmd(QString);

public slots:
    void shellRestart();
    
private slots:
    void on_stackedWidget_currentChanged(int arg1);
    void on_toSingle_clicked();
    void on_toMulti_clicked();
    void shellOutput();
    void shellStderr();
    void shellAskRestart();
    void shellError(QProcess::ProcessError);
    void shellStarted();
    void runInput();
    void delayedInit();
    void viewHistory();

    void delayedOutput();

private:
    bool delayedOutputOn;
    QStringList outputBuffer;
    QString cwd;
    QStringList cmdList;
    int cursor;
    QProcess shell;
    QAction enter,ctrlEnter,prev,next;

    void output(const QString& msg,const QString& color="");

    Ui::Console *ui;
    void resizeEvent(QResizeEvent *);
signals:
    void cwdChanged(QString);
};

#endif // Console_H
