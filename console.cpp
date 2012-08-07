#include "console.h"

#include "ui_console.h"
#include <QTextCodec>
#include <QTimer>


Console::Console(QWidget *parent) :
    QWidget(parent),enter(this),ctrlEnter(this),prev(this),next(this),
    ui(new Ui::Console)
{
    ui->setupUi(this);

    shell.setEnvironment(QProcess::systemEnvironment());
    connect(&shell,SIGNAL(readyReadStandardOutput()),SLOT(shellOutput()));
    connect(&shell,SIGNAL(readyReadStandardError()),SLOT(shellStderr()));
    connect(&shell,SIGNAL(error(QProcess::ProcessError)),SLOT(shellError(QProcess::ProcessError)));
    connect(&shell,SIGNAL(started()),SLOT(shellStarted()));
    connect(&shell,SIGNAL(finished(int)),SLOT(shellRestart()));

    addActions(QList<QAction*>()<<&enter<<&ctrlEnter<<&prev<<&next);
    prev.setShortcut(QKeySequence::MoveToPreviousPage);
    next.setShortcut(QKeySequence::MoveToNextPage);
    connect(&prev,SIGNAL(triggered()),SLOT(viewHistory()));
    connect(&next,SIGNAL(triggered()),SLOT(viewHistory()));
    enter.setShortcut(QKeySequence("Return"));
    ctrlEnter.setShortcut(QKeySequence("Ctrl+Return"));
    connect(&enter,SIGNAL(triggered()),SLOT(runInput()));
    connect(&ctrlEnter,SIGNAL(triggered()),SLOT(runInput()));

    ui->stackedWidget->setCurrentIndex(0);
    QTimer::singleShot(200,this,SLOT(delayedInit()));

    cursor=-1;

    QVariant var=Cfg::get("benchCwd");
    if(var.isValid())
        cwd=var.toString();
    else
        cwd=".";
}

Console::~Console()
{
    shell.disconnect(this);
    shell.write("quit()\n");
    shell.waitForFinished();
    delete ui;
}

void Console::on_stackedWidget_currentChanged(int)
{
    resizeEvent(0);
}

void Console::delayedInit()
{
    ui->line->setFocus();
    shell.start("python -i");
}

void Console::on_toSingle_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->single);
    ui->line->setText(ui->text->toPlainText());
    ui->line->setFocus();
    ui->line->setCursorPosition(ui->line->text().count());
}

void Console::on_toMulti_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->multi);
    ui->text->setPlainText(ui->line->text());
    ui->text->setFocus();
    ui->text->moveCursor(QTextCursor::End);
}

void Console::resizeEvent(QResizeEvent *)
{
    switch(ui->stackedWidget->currentIndex())
    {
    case 0:
        ui->stackedWidget->resize(width(),ui->line->height());
        break;
    case 1:
        ui->stackedWidget->resize(width(),100);
        break;
    }
    ui->output->resize(width(),height()-ui->stackedWidget->height());
    ui->stackedWidget->move(0,ui->output->height());
}

void Console::shellOutput()
{
    QByteArray raw=shell.readAllStandardOutput();
    QString data=QTextCodec::codecForLocale()->toUnicode(raw);

    // signal outputs
    if(data.left(3)=="~~~")
    {
        data=data.mid(3);
        int p;
        if(-1!=(p=data.indexOf(' ')))
        {
            QString msgType=data.left(p);
            QString msg=data.mid(p+1).trimmed();
            if(msgType=="cwd")
            {
                cwd=msg;
                Cfg::set("benchCwd",msg);
                emit cwdChanged(cwd);
            }
        }
    }else
    {
        ui->output->appendHtml("<font><pre>"+data+"</pre></font>");
        ui->output->moveCursor(QTextCursor::End);
        ui->output->moveCursor(QTextCursor::StartOfLine);
    }
    ui->line->setFocus();
}

void Console::shellStderr()
{
    QByteArray raw=shell.readAllStandardError();
    QString data=QTextCodec::codecForLocale()->toUnicode(raw);
    if(data.left(3)=="...")
        return;
    if(data.right(4).left(3)==">>>")
        data=data.left(data.length()-4);
    ui->output->appendHtml("<font color=\"red\"><pre>"+data+"</pre></font>");
    ui->output->moveCursor(QTextCursor::End);
    ui->output->moveCursor(QTextCursor::StartOfLine);
    ui->line->setFocus();
}

void Console::shellError(QProcess::ProcessError)
{
}

void Console::shellStarted()
{
    shell.write("from os import system as _____sys;");
    shell.write("from os import getcwd as _____cwd;");
    shell.write("from os import chdir as _____cd;");
    shell.write("from os import chdir as _____cd;");
    cd(getCwd());
    ui->output->clear();
}

void Console::shellRestart()
{
    shell.start("python -i");
}

void Console::runInput()
{
    QString cmd;
    if(sender()==&enter)
    {
        if(ui->line->hasFocus())
        {
            cmd=ui->line->text();
            ui->line->clear();
        }else
            return;
    }else if(sender()==&ctrlEnter)
    {
        if(ui->stackedWidget->currentIndex()==1)
        {
            if(ui->text->hasFocus())
            {
                cmd=ui->text->toPlainText();
                ui->text->clear();
                on_toSingle_clicked();
            }
        }else if(ui->line->hasFocus()){
            on_toMulti_clicked();
            ui->text->moveCursor(QTextCursor::End);
            return;
        }else
            return;
    }
    if(!cmd.isEmpty()){
        cmdList.push_front(cmd);
        if(cmdList.count()>15)
        {
            cmdList.pop_back();
        }
        cursor=-1;
    }
    ui->output->appendHtml("<font color=\"blue\"><b><pre>&gt;&gt;&gt; "+cmd+"</b></pre></font>");
    if(!cmd.isEmpty()&&cmd.at(0)==':')
    {
        if(cmd==":cls"||cmd==":clear")
        {
            ui->output->clear();
            return;
        }else if(cmd==":cwd")
        {
            cmd="_____cwd()";
        }else if(cmd.left(4)==":cd ")
        {
            cd(cmd.mid(4));
            return;
        }else if(cmd==":break")
        {
            cmd="^c";
            return;
        }else
        {
            cmd="_______=_____sys(\"\"\" "+cmd.mid(1)+" \"\"\")";
        }
    }
    shell.write(QTextCodec::codecForLocale()->fromUnicode(cmd+'\n'));
}

void Console::cd(QString path)
{
    QString cmd="_____cd('''"+path+"''');print '~~~cwd',_____cwd()";
    shell.write(QTextCodec::codecForLocale()->fromUnicode(cmd+'\n'));
}

QString Console::getCwd()
{
    return cwd;
}

void Console::viewHistory()
{
    if(cmdList.isEmpty())
        return;
    if(sender()==&next)
    {
        cursor--;
        if(cursor<0)
        {
            cursor=0;
        }
    }else if(sender()==&prev)
    {
        cursor++;
        if(cursor>=cmdList.count())
        {
            cursor=cmdList.count()-1;
        }
    }
    QString cmd=cmdList[cursor];
    if(cmd.contains('\n'))
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->text->setPlainText(cmd);
    }else
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->line->setText(cmd);
    }
}
