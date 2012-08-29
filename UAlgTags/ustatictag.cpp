#include "ustatictag.h"

UStaticTag::UStaticTag(QString tag)
{
    this->tag=tag;
    setIndependent(false);
}

void UStaticTag::setText(const QString &text)
{
    QTreeWidgetItem::setText(0,text);
}

void UStaticTag::setIcon(const QIcon & icon)
{
    QTreeWidgetItem::setIcon(0,icon);
}

void UStaticTag::menu(QMenu &)
{

}

QString UStaticTag::tagName() const
{
    return tag;
}
