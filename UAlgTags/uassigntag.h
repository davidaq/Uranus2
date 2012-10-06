#ifndef UASSIGNTAG_H
#define UASSIGNTAG_H

#include "ualgtag.h"
class UArgHook;
class UAssignTag : public UAlgTag
{
    Q_OBJECT
public:
    UAssignTag(bool empty=false);
    virtual void menu(QMenu &);
    virtual QString tagName() const;
    void setVarname(QString);
    void save(QFile &fp, int depth) const;
private slots:
    void menuSetVarname();
private:
    QString varname;
    UArgHook* value;
};

#endif // UASSIGNTAG_H
