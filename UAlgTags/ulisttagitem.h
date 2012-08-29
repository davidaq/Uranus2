#ifndef ULISTTAGITEM_H
#define ULISTTAGITEM_H

#include "uarghook.h"

class UListTagItem : public UArgHook
{
    Q_OBJECT
public:
    UListTagItem();
protected:
    virtual void menu(QMenu &);
private slots:
    void insert();
};

#endif // ULISTTAGITEM_H
