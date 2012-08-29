#include "uarghook.h"
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
        menu.addAction("Set value",this,SLOT(menuSetArgValue()))
        );
    menu.addAction("Use variable",this,SLOT(menuSetArgVar()));
}

void UArgHook::setArgValue(QString value)
{
    arg=value;
    setText(0,tag+": "+arg);
}

void UArgHook::menuSetArgValue()
{
    bool ok;
    QString argS=arg;
    if(!argS.isEmpty())
    {
        if(argS.left(11)=="<variable> ")
            argS=argS.mid(11);
        else{
            if(argS[0]=='"')
                argS=argS.mid(1);
            if(!argS.isEmpty()&&argS[argS.length()-1]=='"')
                argS.chop(1);
            argS=argS.replace(QRegExp("\\\\([\"\\\\])"),"\\1");
        }
    }
    QString input=QInputDialog::getText(0,"Set value","Set value for "+tag,QLineEdit::Normal,argS,&ok);
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

void UArgHook::menuSetArgVar()
{
    bool ok;
    QString argS=arg;
    if(!argS.isEmpty())
    {
        if(argS.left(11)=="<variable> ")
            argS=argS.mid(11);
        else{
            if(argS[0]=='"')
                argS=argS.mid(1);
            if(!argS.isEmpty()&&argS[argS.length()-1]=='"')
                argS.chop(1);
            argS=argS.replace(QRegExp("\\\\([\"\\\\])"),"\\1");
        }
    }
    QString input=QInputDialog::getText(0,"Use variable","Use variable for "+tag,QLineEdit::Normal,argS,&ok);
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
