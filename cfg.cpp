#include "cfg.h"
#include <QFile>
#include <QDataStream>

Cfg Cfg::inst;

Cfg::Cfg()
{
    QFile fp("settings.cfg");
    if(fp.open(QFile::ReadOnly)){
        QDataStream stream(&fp);
        stream>>table;
        fp.close();
    }
}

Cfg::~Cfg()
{
    QFile fp("settings.cfg");
    if(fp.open(QFile::WriteOnly)){
        QDataStream stream(&fp);
        stream<<table;
        fp.close();
    }
}

QVariant Cfg::get(QString key)
{
    if(inst.table.contains(key))
        return inst.table[key];
    else
        return QVariant();
}

void Cfg::set(QString key, QVariant value)
{
    inst.table[key]=value;
}
