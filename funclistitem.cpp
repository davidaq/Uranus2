#include "funclistitem.h"
#include <QtGui>
#include <QTimer>

FuncListItem::FuncListItem(QTreeWidget *parent,int type) :
    QTreeWidgetItem((QTreeWidget *)0,type)
{
    setFlags(flags()|Qt::ItemIsEditable);
    connect(parent,SIGNAL(itemChanged(QTreeWidgetItem*,int)),SLOT(nameChanged()));
    lockCount=0;
    parentWidget=parent;
}

void FuncListItem::setFunctionName(QString name)
{
    functionName=name;
    setText(functionName+"("+arguments.join(",")+")");
}

void FuncListItem::setHintText(QString text)
{
    hintText=text;
    lock();
    setToolTip(0,hintText);
    unlock();
    QRegExp exp("%([^%]+)%");
    int pos=0;
    arguments.clear();
    while(-1!=(pos=exp.indexIn(hintText,pos+1)))
    {
        arguments<<exp.cap(1);
        pos+=exp.matchedLength();
    }
    setText(functionName+"("+arguments.join(",")+")");
}

void FuncListItem::dlgEdit()
{
    QDialog dlg;
    dlg.setWindowTitle("Function setting");
    dlg.setMaximumWidth(300);
    dlg.setMinimumWidth(300);
    dlg.setMaximumHeight(200);
    dlg.setMinimumHeight(200);
    dlg.resize(300,200);
    QLineEdit funcName;
    funcName.setParent(&dlg);
    funcName.setText(functionName);
    funcName.resize(300,30);
    QPlainTextEdit text(&dlg);
    text.move(0,30);
    text.resize(300,120);
    text.setPlainText(hintText);
    QLabel lbl("Indicate argument with %argument name%",&dlg);
    lbl.move(5,150);
    lbl.setStyleSheet("color:#777");
    QPushButton ok("OK",&dlg);
    ok.move(100,170);
    QPushButton cancel("Cancel",&dlg);
    cancel.move(200,170);

    dlg.connect(&ok,SIGNAL(clicked()),&dlg,SLOT(accept()));
    dlg.connect(&cancel,SIGNAL(clicked()),&dlg,SLOT(reject()));

    if(QDialog::Accepted==dlg.exec())
    {
        setFunctionName(funcName.text());
        setHintText(text.toPlainText());
    }
}

void FuncListItem::nameChanged()
{
    if(isLocked())
        return;
    setFunctionName(text(0));
}

QString FuncListItem::getFunctionName()
{
    return functionName;
}

void FuncListItem::edit()
{
    setText(functionName);
    parentWidget->editItem(this);
}

void FuncListItem::setText(const QString &text)
{
    lock();
    QTreeWidgetItem::setText(0,text);
    unlock();
}

void FuncListItem::lock()
{
    lockCount=1;
}

void FuncListItem::unlock()
{
    lockCount=0;
}

bool FuncListItem::isLocked()
{
    return lockCount>0;
}
