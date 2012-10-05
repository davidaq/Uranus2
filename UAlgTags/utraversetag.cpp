#include "utraversetag.h"

UTraverseTag::UTraverseTag(bool empty)
{
    setIndependent(true);
    setText(0,"Loop");
    setIcon(0,QIcon(":/images/algorithm/loop.png"));
    if(empty)
        return;
    condition=new UArgHook;
    condition->setIndependent(false);
    condition->setTagName("Traversed list");
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

void UTraverseTag::save(QFile &fp, int depth) const
{
    writeLine(fp,"traverse",depth);
    UAlgTag::save(fp,depth);
}
