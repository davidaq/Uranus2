#ifndef UCOMMENTTAG_H
#define UCOMMENTTAG_H

#include "ualgtag.h"

class UCommentTag : public UAlgTag
{
    Q_OBJECT
public:
    explicit UCommentTag();
    void setComment(QString);
    QString comment() const;
    virtual void menu(QMenu &);
    virtual QString tagName() const;
    void save(QFile &fp, int depth) const;
private:
signals:
    void hintChanged(QString);
public slots:
    void edit();
};

#endif // UCOMMENTTAG_H
