#include "ulooptag.h"

ULoopTag::ULoopTag(bool empty)
{
    setIndependent(true);
    setText(0,"Loop");
    setIcon(0,QIcon(":/images/algorithm/loop.png"));
    if(!empty){
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
}

void ULoopTag::menu(QMenu &)
{

}

QString ULoopTag::tagName() const
{
    return "loop";
}

void ULoopTag::save(QFile &fp, int depth) const
{
    writeLine(fp,"loop",depth);
    UAlgTag::save(fp,depth);
}
