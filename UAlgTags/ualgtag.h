#ifndef UALGTAG_H
#define UALGTAG_H

#include <QTreeWidgetItem>
#include <QObject>

class QMenu;
class UAlgTag : public QObject,public QTreeWidgetItem
{
    Q_OBJECT
public:
    UAlgTag();
    ~UAlgTag();
    virtual QString tagName() const =0;
    virtual void expanded(bool);
    bool isIndependent() const;
    void setIndependent(bool);
    void callMenu(QMenu&);
    void insertAfter(UAlgTag*);
    int index();
    void insertChild(int index, UAlgTag *child);
    void addChild(UAlgTag *child);
    void setText(int column, const QString &text);
public slots:
    void trigger();
protected:
    virtual void menu(QMenu&)=0;
private:
    bool independent;
    bool aboutToExpand;
    void expandDelayed();
private slots:
    void insertReturn();
    void expandDelayedS();
};

#include "ustatictag.h"
#include "ucommenttag.h"
#include "ucontainertag.h"
#include "uarghook.h"
#include "ucalltag.h"
#include "uassigntag.h"
#include "ulooptag.h"
#include "utraversetag.h"
#include "uconditiontag.h"
#include "ufunctiontag.h"

#endif // UALGTAG_H
