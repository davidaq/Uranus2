#ifndef UBRANCHTAG_H
#define UBRANCHTAG_H

#include "ualgtag.h"
class UBranchTag : public UAlgTag
{
    Q_OBJECT
public:
    UBranchTag(bool empty=false);
    virtual QString tagName() const;
    void save(QFile &fp, int depth) const;
protected:
    virtual void menu(QMenu &);
private:
    UArgHook* condition;
    UContainerTag* action;
private slots:
    void insertBranch();
};

#endif // UBRANCHTAG_H
