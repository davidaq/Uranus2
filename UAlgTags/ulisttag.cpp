#include "ulisttag.h"
#include "ulisttagitem.h"
#include <QMenu>

UListTag::UListTag()
{
    setIndependent(false);
    setIcon(0,QIcon(":/images/algorithm/items.png"));
    setText(0,"List");
}

void UListTag::menu(QMenu &menu)
{
    menu.addAction("Add item",this,SLOT(addItem()));
    if(childCount()>0)
        menu.addAction("Clear list",this,SLOT(clear()));
}

QString UListTag::tagName() const
{
    return "list";
}

void UListTag::addItem()
{
    UListTagItem* item=new UListTagItem;
    item->setTagName("item");
    addChild(item);
}

void UListTag::clear()
{
    foreach(QTreeWidgetItem* item,takeChildren())
    {
        delete item;
    }
}

void UListTag::expanded(bool exp)
{
    if(exp)
    {
        setText(0,"List");
    }else
    {
        QStringList list;
        for(int i=0,c=childCount();i<c;i++)
        {
            UArgHook* item=dynamic_cast<UArgHook*>(child(i));
            if(item)
            {
                list<<item->value();
            }
        }
        setText(0,"[ "+list.join(" , ")+" ]");
    }
}

void UListTag::save(QFile &fp, int depth) const
{
    writeLine(fp,"list",depth);
    UAlgTag::save(fp,depth);
}
