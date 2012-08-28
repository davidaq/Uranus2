#ifndef UALGTAG_H
#define UALGTAG_H

#include <QTreeWidgetItem>
#include <QObject>

class QMenu;
class UAlgTag : public QTreeWidgetItem,public QObject
{
public:
    virtual QString tagName() const =0;
    virtual void menu(QMenu&)=0;
};

#include "ucommenttag.h"
#include "ucontainertag.h"
#include "uarghook.h"
#include "ufunctiontag.h"

#endif // UALGTAG_H
