#ifndef UALGTAG_H
#define UALGTAG_H

#include <QTreeWidgetItem>
#include <QObject>
class QFile;
extern void writeLine(QFile &fp,QString data,int indent=0);
class QMenu;
class UAlgTag : public QObject,public QTreeWidgetItem
{
    Q_OBJECT
public:
    UAlgTag();
    ~UAlgTag();
    virtual QString tagName() const =0;
    virtual void save(QFile &fp,int depth=0) const;
    virtual void expanded(bool);
    bool isIndependent() const;
    void setIndependent(bool);
    void callMenu(QMenu&);
    void insertAfter(UAlgTag*);
    int index();
    void insertChild(int index, UAlgTag *child);
    void addChild(UAlgTag *child);
    void setText(int column, const QString &text);
    int getDepth() const;
public slots:
    void trigger();
protected:
    virtual void menu(QMenu&)=0;
private:
    int depth;
    bool independent;
    bool aboutToExpand;
    void expandDelayed();
private slots:
    void addReturn();
    void addComment();
    void addBreak();
    void addContinue();
    void addAssign();
    void addExec();
    void addLoop();
    void addTraverse();
    void addCondition();
    void addFunction();
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
