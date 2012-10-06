#include "uarghook.h"
#include "ulisttag.h"
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

UArgHook::UArgHook()
{
    tag="arg";
    setIndependent(false);
}

QString UArgHook::tagName() const
{
    return tag;
}

void UArgHook::setTagName(QString name)
{
    tag=name;
    setText(0,tag+": "+arg);
}

void UArgHook::menu(QMenu &menu)
{
    menu.setDefaultAction(
        menu.addAction("Set as value",this,SLOT(menuSetArgValue()))
        );
    menu.addAction("Set as list",this,SLOT(menuSetArgList()));
    menu.addAction("Use variable",this,SLOT(menuSetArgVar()));
    menu.addAction("Use traverse value",this,SLOT(menuSetTraverse()));
    menu.addAction("Use function return value",this,SLOT(menuSetArgFunction()));
}

void UArgHook::setArgValue(QString value)
{
    arg=value;
    setText(0,tag+": "+arg);
    delete takeChild(0);
}

QString argProc(QString arg)
{
    QString argS=arg;
    if(!argS.isEmpty())
    {
        if(argS=="<function>"||argS=="<list>")
            argS="";
        else if(argS.left(11)=="<variable> ")
            argS=argS.mid(11);
        else{
            if(argS[0]=='"')
                argS=argS.mid(1);
            if(!argS.isEmpty()&&argS[argS.length()-1]=='"')
                argS.chop(1);
            argS=argS.replace(QRegExp("\\\\([\"\\\\])"),"\\1");
        }
    }
    return argS;
}

void UArgHook::menuSetArgValue()
{
    bool ok;
    QString input=QInputDialog::getText(0,"Set value","Set value for "+tag,QLineEdit::Normal,argProc(arg),&ok);
    if(ok)
    {
        if(input.isEmpty())
        {
            setArgValue("");
            return;
        }
        input.toDouble(&ok);
        if(!ok&&(input[0]!='"'||input[input.length()-1]!='"'))
        {
            input=input.replace(QRegExp("([\"\\\\])"),"\\\\1");
            input='"'+input+'"';
        }
        setArgValue(input);
    }

}

void UArgHook::menuSetTraverse()
{
    setArgValue("<variable> traverse");
}

void UArgHook::menuSetArgVar()
{
    bool ok;
    QString input=QInputDialog::getText(0,"Use variable","Use variable for "+tag,QLineEdit::Normal,argProc(arg),&ok);
    if(ok)
    {
        if(input.isEmpty())
        {
            setArgValue("");
            return;
        }
        setArgValue("<variable> "+input);
    }
}

QString UArgHook::value() const
{
    return arg;
}

void UArgHook::menuSetArgFunction()
{
    setArgValue("<function>");
    UCallTag* item=new UCallTag;
    item->setIndependent(false);
    addChild(item);
}

void UArgHook::menuSetArgList()
{
    setArgValue("<list>");
    UListTag* item=new UListTag;
    addChild(item);
}

void UArgHook::save(QFile &fp, int depth) const
{
    writeLine(fp,tagName()+" "+value(),depth);
    UAlgTag::save(fp,depth);
}
