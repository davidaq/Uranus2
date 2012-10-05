#ifndef UTRAVERSETAG_H
#define UTRAVERSETAG_H

#include "ualgtag.h"
class UTraverseTag : public UAlgTag
{
    Q_OBJECT
public:
    UTraverseTag(bool empty=false);
    virtual QString tagName() const;
    void save(QFile &fp, int depth) const;
protected:
    virtual void menu(QMenu &);
private:
    UArgHook* condition;
    UContainerTag* action;
};

#endif // UTRAVERSETAG_H
