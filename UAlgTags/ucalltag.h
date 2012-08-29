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
public slots:
    void selectFunction();
private:
    QString modulePath;
    QString functionName;
signals:
    
private slots:
    
};

#endif // UCALLTAG_H
