#include "ucommenttag.h"
#include <QInputDialog>
#include <QMenu>

UCommentTag::UCommentTag() :
    UAlgTag()
{
    setIcon(0,QIcon(":/images/algorithm/tag.png"));
}

void UCommentTag::setComment(QString comment)
{
    setText(0,comment);
}

QString UCommentTag::comment()
{
    return text(0);
}

void UCommentTag::menu(QMenu &menu)
{
    menu.addAction("Set comment",this,SLOT(edit()));
}

QString UCommentTag::tagName() const
{
    return "hint";
}

void UCommentTag::edit()
{

    bool ok;
    QString input=QInputDialog::getText(0,"Edit comment","Comment:",QLineEdit::Normal,comment(),&ok);
    if(ok)
    {
        setComment(input);
    }
}
