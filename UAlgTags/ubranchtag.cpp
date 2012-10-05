#include "ubranchtag.h"
#include <QMenu>

UBranchTag::UBranchTag(bool empty)
{
    setIndependent(false);
    setText(0,"Branch");
    setIcon(0,QIcon(":/images/algorithm/branch.png"));
    if(!empty){
        condition=new UArgHook;
        condition->setIndependent(false);
        condition->setTagName("condition");
        condition->setIcon(0,QIcon(":/images/algorithm/condition.png"));
        condition->setArgValue("1");
        addChild(condition);
        action=new UContainerTag;
        action->setIndependent(false);
        action->setText(0,"action");
        addChild(action);
    }
}

void UBranchTag::menu(QMenu &menu)
{
    menu.addAction("Insert branch",this,SLOT(insertBranch()));
    menu.addSeparator();
    menu.addAction("Delete",this,SLOT(deleteLater()));
}

QString UBranchTag::tagName() const
{
    return "branch";
}

void UBranchTag::insertBranch()
{
    UAlgTag* p=dynamic_cast<UAlgTag*>(QTreeWidgetItem::parent());
    if(p)
        p->insertChild(index(),new UBranchTag);
}

void UBranchTag::save(QFile &fp, int depth) const
{
    writeLine(fp,"branch",depth);
    UAlgTag::save(fp,depth);
}
