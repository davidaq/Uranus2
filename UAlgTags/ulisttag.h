#ifndef ULISTTAG_H
#define ULISTTAG_H

#include "ualgtag.h"

class UListTag : public UAlgTag
{
    Q_OBJECT
public:
    UListTag();
    virtual QString tagName() const;
    virtual void expanded(bool);
    void save(QFile &fp, int depth) const;
protected:
    virtual void menu(QMenu &);
private slots:
    void addItem();
    void clear();
};

#endif // ULISTTAG_H
