#ifndef UCONDITIONTAG_H
#define UCONDITIONTAG_H

#include "ualgtag.h"

class UConditionTag : public UAlgTag
{
    Q_OBJECT
public:
    UConditionTag(bool empty=false);
    virtual QString tagName() const;
    void save(QFile &fp, int depth) const;
protected:
    virtual void menu(QMenu &);
private slots:
    void addBranch();
};

#endif // UCONDITIONTAG_H
