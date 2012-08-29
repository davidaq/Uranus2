#include "ucontainertag.h"
#include <QMenu>

UContainerTag::UContainerTag()
{
    tag="do";
    setIndependent(true);
    setIcon(0,QIcon(":/images/algorithm/code.png"));
}

QString UContainerTag::tagName() const
{
    return tag;
}

void UContainerTag::setTagName(QString name)
{
    tag=name;
}

void UContainerTag::menu(QMenu &menu)
{
    QMenu *m;
    if(isIndependent())
    {
        m=menu.addMenu("Insert within");
    }else
        m=&menu;
    m->addAction("  - Insert -")->setEnabled(false);
    m->addAction("Condition",this,SLOT(addCondition()));
    m->addAction("Traverse",this,SLOT(addTraverse()));
    m->addAction("Loop",this,SLOT(addLoop()));
    m->addAction("Break loop",this,SLOT(addBreak()));
    m->addAction("Continue loop",this,SLOT(addContinue()));
    m->addAction("Function call",this,SLOT(addFunction()));
    m->addAction("Variable assign",this,SLOT(addAssign()));
    m->addAction("Return",this,SLOT(addReturn()));
    m->addAction("Python command",this,SLOT(addExec()));
    m->addAction("Comment",this,SLOT(addComment()));
}

void UContainerTag::addReturn()
{
    UArgHook* item=new UArgHook;
    item->setIcon(0,QIcon(":/images/algorithm/return.png"));
    item->setTagName("return");
    item->setIndependent(true);
    insertChild(0,item);
}

void UContainerTag::addComment()
{
    UCommentTag* item=new UCommentTag;
    insertChild(0,item);
}

void UContainerTag::addBreak()
{
    UStaticTag* item=new UStaticTag("break");
    item->setIcon(QIcon(":/images/algorithm/break.png"));
    item->setText("break");
    item->setIndependent(true);
    insertChild(0,item);
}

void UContainerTag::addContinue()
{
    UStaticTag* item=new UStaticTag("break");
    item->setIcon(QIcon(":/images/algorithm/continue.png"));
    item->setText("continue");
    item->setIndependent(true);
    insertChild(0,item);
}

void UContainerTag::addAssign()
{
    UAssignTag* item=new UAssignTag;
    insertChild(0,item);
}

void UContainerTag::addExec()
{
    UArgHook* item=new UArgHook;
    item->setIcon(0,QIcon(":/images/algorithm/command.png"));
    item->setTagName("execute");
    item->setIndependent(true);
    insertChild(0,item);
}

void UContainerTag::addLoop()
{
    ULoopTag* item=new ULoopTag;
    insertChild(0,item);
}

void UContainerTag::addTraverse()
{
    UTraverseTag *item=new UTraverseTag;
    insertChild(0,item);
}

void UContainerTag::addCondition()
{
    UConditionTag *item=new UConditionTag;
    insertChild(0,item);
}

void UContainerTag::addFunction()
{
    UCallTag* item=new UCallTag;
    insertChild(0,item);
}
