#ifndef UFUNCTIONTAG_H
#define UFUNCTIONTAG_H

#include "ualgtag.h"
#include <QObject>
class UFunctionTag : public QObject,public UAlgTag
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
    QString funcName,hint,args;
    QTreeWidgetItem *hintTag,*body;
private slots:
    void editName();
    void editHint();
    void editArgs();
};

#endif // UFUNCTIONTAG_H
