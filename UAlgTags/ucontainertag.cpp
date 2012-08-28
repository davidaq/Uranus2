#include "ucontainertag.h"
#include <QMenu>

UContainerTag::UContainerTag()
{
    tag="do";
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
    menu.addAction("  - Insert -")->setEnabled(false);
    menu.addAction("Condition");
    menu.addAction("Traverse");
    menu.addAction("Loop");
    menu.addAction("Break loop");
    menu.addAction("Continue loop");
    menu.addAction("Assign");
    menu.addAction("Return",this,SLOT(addReturn()));
    menu.addAction("Python command");
    menu.addAction("Comment");
}

void UContainerTag::addReturn()
{
    UArgHook* item=new UArgHook;
    item->setIcon(0,QIcon(":/images/algorithm/return.png"));
    item->setTagName("return");
    item->setIndependent(true);
    addChild(item);
}
