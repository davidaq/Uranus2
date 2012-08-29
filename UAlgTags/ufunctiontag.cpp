#include "ufunctiontag.h"
#include <QMenu>
#include <QInputDialog>

UFunctionTag::UFunctionTag()
{
    setIcon(0,QIcon(":/images/algorithm/function.png"));
    hintTag=new UCommentTag;
    hintTag->setIndependent(false);
    hintTag->setIcon(0,QIcon(":/images/algorithm/tag.png"));
    setIndependent(false);
    connect(hintTag,SIGNAL(hintChanged(QString)),SIGNAL(hintChanged(QString)));
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
    emit nameChanged(name);
}

void UFunctionTag::setHint(QString text)
{
    hintTag->setComment(text);
}

void UFunctionTag::setArgs(const QStringList& list)
{
    removeChild(hintTag);
    foreach(QTreeWidgetItem* item,takeChildren())
    {
        delete item;
    }

    addChild(hintTag);
    QMutableStringListIterator iterator(*((QList<QString>*)&list));
    while(iterator.hasNext())
    {
        QString item=iterator.next();
        item=item.trimmed();
        UStaticTag* child=new UStaticTag("arg");
        child->setIcon(QIcon(":/images/algorithm/var.png"));
        child->setText(item);
        addChild(child);
        iterator.setValue(item);
    }
    args=list.join(",");
    emit argsChanged(list);
}

void UFunctionTag::menu(QMenu& menu)
{
    menu.setDefaultAction(
        menu.addAction(QIcon(":/images/algorithm/function.png"),"Edit function name",this,SLOT(editName()))
        );
    menu.addAction(QIcon(":/images/algorithm/tag.png"),"Edit function hint",hintTag,SLOT(edit()));
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

void UFunctionTag::editArgs()
{
    bool ok;
    QString input=QInputDialog::getText(0,"Edit arguments list","Argument list: [Separate with ,]",QLineEdit::Normal,args,&ok);
    if(ok)
    {
        setArgs(input.split(','));
    }
}
