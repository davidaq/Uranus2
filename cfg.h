#ifndef CFG_H
#define CFG_H

#include <QVariant>
#include <QMap>
class Cfg
{
private:
    Cfg();
    ~Cfg();
    QMap<QString,QVariant> table;
    static Cfg inst;
public:
    static QVariant get(QString);
    static void set(QString,QVariant);
};

#endif // CFG_H
