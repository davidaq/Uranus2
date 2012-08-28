#ifndef UALGTAG_H
#define UALGTAG_H

#include <QTreeWidgetItem>

class QMenu;
class UAlgTag : public QTreeWidgetItem
{
public:
    virtual QString tagName() const =0;
    virtual void menu(QMenu&)=0;
};

#include "ufunctiontag.h"
#include "ustatictag.h"

#endif // UALGTAG_H
