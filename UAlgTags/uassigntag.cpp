#include "uassigntag.h"
#include <QMenu>
#include <QInputDialog>

UAssignTag::UAssignTag()
{
    value=new UArgHook;
    value->setTagName("value");
    addChild(value);
    setIndependent(true);
    setText(0,"Assign :");
    setIcon(0,QIcon(":/images/algorithm/var.png"));
    value->setIcon(0,QIcon(":/images/algorithm/var.png"));
}

void UAssignTag::menu(QMenu &menu)
{
    menu.setDefaultAction(
                menu.addAction("Assign variable",this,SLOT(menuSetVarname()))
        );
    QMenu *m=menu.addMenu("Value");
    value->menu(*m);
}

QString UAssignTag::tagName() const
{
    return "assign";
}

void UAssignTag::menuSetVarname()
{
    bool ok;
    QString input=QInputDialog::getText(0,"Assign variable","Variable name:",QLineEdit::Normal,varname,&ok);
    if(ok)
    {
        setVarname(input);
    }
}

void UAssignTag::setVarname(QString var)
{
    varname=var;
    setText(0,"Assign "+var+":");
}
