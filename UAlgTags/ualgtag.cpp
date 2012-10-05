#include "ualgtag.h"
#include <QMenu>
#include <QTimer>

UAlgTag::UAlgTag()
{
    independent=true;
    aboutToExpand=false;
    depth = 0;
}

UAlgTag::~UAlgTag()
{
    foreach(QTreeWidgetItem* item,takeChildren())
    {
        delete item;
    }
}

void UAlgTag::setIndependent(bool b)
{
    independent=b;
}

bool UAlgTag::isIndependent() const
{
    return independent;
}

int UAlgTag::index()
{
    UAlgTag *p=dynamic_cast<UAlgTag *>(QTreeWidgetItem::parent());
    if(p)
    {
        return p->indexOfChild(this);
    }else
        return -1;
}

void UAlgTag::insertAfter(UAlgTag *item)
{
    UAlgTag *p=dynamic_cast<UAlgTag *>(QTreeWidgetItem::parent());
    if(p)
    {
        p->insertChild(p->indexOfChild(this)+1,item);
    }
}

void UAlgTag::callMenu(QMenu &menu)
{
    this->menu(menu);
    if(isIndependent())
    {
        menu.addSeparator();
        QMenu *m=menu.addMenu("Insert after");
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
        menu.addSeparator();
        menu.addAction("Delete",this,SLOT(deleteLater()));
    }
}

void UAlgTag::trigger()
{
    QMenu tempMenu;
    menu(tempMenu);
    QAction* act=tempMenu.defaultAction();
    if(act)
        act->trigger();
    else
        setExpanded(!isExpanded());
}

void UAlgTag::insertChild(int index, UAlgTag *child)
{
    QTreeWidgetItem::insertChild(index,child);
    child->depth = depth+1;
    expandDelayed();
}

void UAlgTag::addChild(UAlgTag *child)
{
    QTreeWidgetItem::addChild(child);
    child->depth = depth+1;
    expandDelayed();
}

void UAlgTag::setText(int column, const QString &text)
{
    QTreeWidgetItem::setText(column,text);
    setToolTip(column,text);
}

void UAlgTag::expandDelayed()
{
    if(!aboutToExpand)
    {
        QTimer::singleShot(100,this,SLOT(expandDelayedS()));
        aboutToExpand=true;
    }
}

void UAlgTag::expandDelayedS()
{
    setExpanded(true);
    aboutToExpand=false;
}

void UAlgTag::expanded(bool)
{

}

void UAlgTag::addReturn()
{
    UArgHook* item=new UArgHook;
    item->setIcon(0,QIcon(":/images/algorithm/return.png"));
    item->setTagName("return");
    item->setIndependent(true);
    insertAfter(item);
}

void UAlgTag::addComment()
{
    UCommentTag* item=new UCommentTag;
    insertAfter(item);
}

void UAlgTag::addBreak()
{
    UStaticTag* item=new UStaticTag("break");
    item->setIcon(QIcon(":/images/algorithm/break.png"));
    item->setText("break");
    item->setIndependent(true);
    insertAfter(item);
}

void UAlgTag::addContinue()
{
    UStaticTag* item=new UStaticTag("break");
    item->setIcon(QIcon(":/images/algorithm/continue.png"));
    item->setText("continue");
    item->setIndependent(true);
    insertAfter(item);
}

void UAlgTag::addAssign()
{
    UAssignTag* item=new UAssignTag;
    insertAfter(item);
}

void UAlgTag::addExec()
{
    UArgHook* item=new UArgHook;
    item->setIcon(0,QIcon(":/images/algorithm/command.png"));
    item->setTagName("execute");
    item->setIndependent(true);
    insertAfter(item);
}

void UAlgTag::addLoop()
{
    ULoopTag* item=new ULoopTag;
    insertAfter(item);
}

void UAlgTag::addTraverse()
{
    UTraverseTag *item=new UTraverseTag;
    insertAfter(item);
}

void UAlgTag::addCondition()
{
    UConditionTag *item=new UConditionTag;
    insertAfter(item);
}

void UAlgTag::addFunction()
{
    UCallTag* item=new UCallTag;
    insertAfter(item);
}

void UAlgTag::save(QFile &fp, int depth) const
{
    depth++;
    for(int i=0,c=childCount();i<c;i++){
        UAlgTag* item = dynamic_cast<UAlgTag*>(child(i));
        if(item!=0)
            item->save(fp,depth);
    }
}

int UAlgTag::getDepth() const{
    return depth;
}
