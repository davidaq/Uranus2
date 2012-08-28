#include "ufunctiontag.h"
#include <QMenu>
#include <QInputDialog>

UFunctionTag::UFunctionTag()
{
    setIcon(0,QIcon(":/images/algorithm/function.png"));
    hintTag=new UStaticTag;
    hintTag->setIcon(0,QIcon(":/images/algorithm/tag.png"));
    body=new UStaticTag;
    body->setIcon(0,QIcon(":/images/algorithm/code.png"));
    body->setText(0,"Action");
    addChild(body);
}

QString UFunctionTag::tagName() const
{
    return "function";
}

void UFunctionTag::setName(QString name)
{
    funcName=name;
    setText(0,name);
    setExpanded(true);
}

void UFunctionTag::setHint(QString text)
{
    hint=text;
    hintTag->setText(0,text);
}

void UFunctionTag::setArgs(const QStringList& list)
{
    removeChild(hintTag);
    removeChild(body);
    foreach(QTreeWidgetItem* item,takeChildren())
    {
        delete item;
    }

    addChild(hintTag);
    foreach(QString item,list)
    {
        QTreeWidgetItem* child=new QTreeWidgetItem;
        child->setIcon(0,QIcon(":/images/algorithm/var.png"));
        child->setText(0,item);
        addChild(child);
    }
    addChild(body);
    args=list.join(";");
}

void UFunctionTag::menu(QMenu& menu)
{
    menu.addAction(QIcon(":/images/algorithm/function.png"),"Edit function name",this,SLOT(editName()));
    menu.addAction(QIcon(":/images/algorithm/tag.png"),"Edit function hint",this,SLOT(editHint()));
    menu.addAction(QIcon(":/images/algorithm/var.png"),"Edit arguments list",this,SLOT(editArgs()));
}

void UFunctionTag::editName()
{
    bool ok;
    QString input=QInputDialog::getText(0,"Edit function name","Name:",QLineEdit::Normal,funcName,&ok);
    if(ok&&!input.isEmpty())
    {
        setName(input);
    }
}

void UFunctionTag::editHint()
{
    bool ok;
    QString input=QInputDialog::getText(0,"Edit function hint","Hint text:",QLineEdit::Normal,hint,&ok);
    if(ok)
    {
        setHint(input);
    }
}

void UFunctionTag::editArgs()
{
    bool ok;
    QString input=QInputDialog::getText(0,"Edit arguments list","Argument list: [Separate with ;]",QLineEdit::Normal,args,&ok);
    if(ok)
    {
        setArgs(input.split(';'));
    }
}
