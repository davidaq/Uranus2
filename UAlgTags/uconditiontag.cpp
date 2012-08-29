#include "uconditiontag.h"
#include "ubranchtag.h"
#include <QMenu>

UConditionTag::UConditionTag()
{
    setIndependent(true);
    setText(0,"Conditional branching");
    setIcon(0,QIcon(":/images/algorithm/switch.png"));
    UBranchTag* branch=new UBranchTag;
    addChild(branch);
}

void UConditionTag::menu(QMenu &menu)
{
    menu.addAction("Append branch",this,SLOT(addBranch()));
}

QString UConditionTag::tagName() const
{
    return "if";
}

void UConditionTag::addBranch()
{
    UBranchTag* branch=new UBranchTag;
    addChild(branch);
}
