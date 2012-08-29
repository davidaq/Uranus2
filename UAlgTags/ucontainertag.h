#ifndef UCONTAINERTAG_H
#define UCONTAINERTAG_H

#include "ualgtag.h"

class UContainerTag : public UAlgTag
{
    Q_OBJECT
public:
    UContainerTag();
    virtual QString tagName() const;
    virtual void menu(QMenu&);
    void setTagName(QString);
private:
    QString tag;

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
};

#endif // UCONTAINERTAG_H
