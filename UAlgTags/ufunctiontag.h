#ifndef UFUNCTIONTAG_H
#define UFUNCTIONTAG_H

#include "ualgtag.h"
class UCommentTag;
class UFunctionTag : public UAlgTag
{
    Q_OBJECT
public:
    UFunctionTag();
    virtual QString tagName() const;
    void setName(QString);
    void setHint(QString);
    void setArgs(const QStringList&);
    virtual void menu(QMenu&);
private:
    QString funcName,args;
    UCommentTag *hintTag;
private slots:
    void editName();
    void editArgs();
signals:
    void nameChanged(QString);
    void hintChanged(QString);
    void argsChanged(QStringList);
};

#endif // UFUNCTIONTAG_H
