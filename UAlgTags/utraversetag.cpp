#include "utraversetag.h"

UTraverseTag::UTraverseTag()
{
    setIndependent(true);
    setText(0,"Loop");
    setIcon(0,QIcon(":/images/algorithm/loop.png"));
    condition=new UArgHook;
    condition->setIndependent(false);
    condition->setTagName("list");
    condition->setIcon(0,QIcon(":/images/algorithm/items.png"));
    addChild(condition);
    action=new UContainerTag;
    action->setIndependent(false);
    action->setText(0,"action");
    addChild(action);
}

QString UTraverseTag::tagName() const
{
    return "traverse";
}

void UTraverseTag::menu(QMenu &)
{

}
