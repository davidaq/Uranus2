#include "ualgtag.h"
#include <QMenu>
#include <QTimer>

UAlgTag::UAlgTag()
{
    independent=true;
    aboutToExpand=false;
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
        m->addAction("Return",this,SLOT(insertReturn()));
        menu.addSeparator();
        menu.addAction("Delete",this,SLOT(deleteLater()));
    }
}

void UAlgTag::insertReturn()
{
    UArgHook* item=new UArgHook;
    item->setIcon(0,QIcon(":/images/algorithm/return.png"));
    item->setTagName("return");
    item->setIndependent(true);
    insertAfter(item);
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
    expandDelayed();
}

void UAlgTag::addChild(UAlgTag *child)
{
    QTreeWidgetItem::addChild(child);
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
