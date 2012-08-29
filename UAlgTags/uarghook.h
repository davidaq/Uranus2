#ifndef UARGHOOK_H
#define UARGHOOK_H

#include "ualgtag.h"
#include <QDebug>

class UArgHook : public UAlgTag
{
    Q_OBJECT
public:
    UArgHook();
    virtual QString tagName() const;
    virtual void menu(QMenu &);
    void setTagName(QString);
    void setArgValue(QString);
    void setArgFunction();
private:
    QString tag,arg;
private slots:
    void menuSetArgValue();
    void menuSetArgVar();
};

#endif // UARGHOOK_H
