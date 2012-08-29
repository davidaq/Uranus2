#include "ulooptag.h"

ULoopTag::ULoopTag()
{
    setIndependent(true);
    setText(0,"Loop");
    setIcon(0,QIcon(":/images/algorithm/loop.png"));
    condition=new UArgHook;
    condition->setIndependent(false);
    condition->setTagName("condition");
    condition->setIcon(0,QIcon(":/images/algorithm/condition.png"));
    addChild(condition);
    action=new UContainerTag;
    action->setIndependent(false);
    action->setText(0,"action");
    addChild(action);
}

void ULoopTag::menu(QMenu &)
{

}

QString ULoopTag::tagName() const
{
    return "loop";
}
