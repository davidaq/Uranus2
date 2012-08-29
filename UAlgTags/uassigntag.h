#ifndef UASSIGNTAG_H
#define UASSIGNTAG_H

#include "ualgtag.h"
class UArgHook;
class UAssignTag : public UAlgTag
{
    Q_OBJECT
public:
    UAssignTag();
    virtual void menu(QMenu &);
    virtual QString tagName() const;
    void setVarname(QString);
private slots:
    void menuSetVarname();
private:
    QString varname;
    UArgHook* value;
};

#endif // UASSIGNTAG_H
