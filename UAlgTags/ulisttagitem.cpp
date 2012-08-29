#include "ulisttagitem.h"
#include <QMenu>

UListTagItem::UListTagItem()
{
    setIndependent(false);
    setIcon(0,QIcon(":/images/algorithm/var.png"));
}

void UListTagItem::menu(QMenu & menu)
{
    UArgHook::menu(menu);
    menu.addSeparator();
    menu.addAction("Insert item after",this,SLOT(insert()));
    menu.addSeparator();
    menu.addAction("Delete",this,SLOT(deleteLater()));
}

void UListTagItem::insert()
{
    UListTagItem* item=new UListTagItem;
    item->setTagName("item");
    insertAfter(item);
}
