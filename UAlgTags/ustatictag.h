#ifndef USTATICTAG_H
#define USTATICTAG_H

#include "ualgtag.h"

class UStaticTag : public UAlgTag
{
    Q_OBJECT
public:
    UStaticTag(QString tag="hint");
    virtual void menu(QMenu &);
    virtual QString tagName() const;
    void setText(const QString&);
    void setIcon(const QIcon&);
    void save(QFile &fp, int depth) const;
private:
    QString tag;
};

#endif // USTATICTAG_H
