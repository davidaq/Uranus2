#ifndef USTATICTAG_H
#define USTATICTAG_H
#include "ualgtag.h"
class UStaticTag : public UAlgTag
{
public:
    UStaticTag();
    virtual QString tagName() const;
    virtual void menu(QMenu&);
};

#endif // USTATICTAG_H
