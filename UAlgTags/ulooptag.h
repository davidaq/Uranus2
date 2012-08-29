#ifndef ULOOPTAG_H
#define ULOOPTAG_H

#include "ualgtag.h"
class UContainerTag;
class ULoopTag : public UAlgTag
{
public:
    ULoopTag();
    virtual void menu(QMenu &);
    virtual QString tagName() const;
private:
    UArgHook* condition;
    UContainerTag* action;
};

#endif // ULOOPTAG_H
