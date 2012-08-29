#include "ucalltag.h"

UCallTag::UCallTag()
{
    setIcon(0,QIcon(":/images/alogorithm/function.png"));
}

void UCallTag::menu(QMenu &)
{

}

QString UCallTag::tagName() const
{
    return "call";
}
