#ifndef UCALLTAG_H
#define UCALLTAG_H

#include "ualgtag.h"

class UCallTag : public UAlgTag
{
    Q_OBJECT
public:
    explicit UCallTag();
    virtual QString tagName() const;
    virtual void menu(QMenu &);
    bool setFunction(const QString &,const QString &);
    void setArgs(QStringList);
public slots:
    void selectFunction();
private:
    QString moduleName,functionName;
    QStringList args;
signals:
    
private slots:
    
};

#endif // UCALLTAG_H
