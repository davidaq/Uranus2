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
    QString value() const;
private:
    QString tag,arg;
private slots:
    void menuSetArgValue();
    void menuSetArgVar();
    void menuSetArgList();
    void menuSetArgFunction();
};

#endif // UARGHOOK_H
