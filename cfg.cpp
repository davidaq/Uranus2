#include "cfg.h"
#include <QFile>
#include <QDataStream>
#include <QDir>

Cfg Cfg::inst;

Cfg::Cfg()
{
    path = QDir::current().absoluteFilePath("settings.cfg");
    QFile fp(path);
    if(fp.open(QFile::ReadOnly)){
        QDataStream stream(&fp);
        stream>>table;
        fp.close();
    }
    tick.setInterval(1500);
    connect(&tick,SIGNAL(timeout()),this,SLOT(alarmListeners()));
}

Cfg::~Cfg()
{
    if(!changed.isEmpty())
        save();
}

void Cfg::save()
{
    QFile fp(path);
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
    inst.changed.insert(key);
}

void Cfg::listen(QString key, QObject *reciever, const char *member)
{
    inst.listeners[key].insert(ListenItem(reciever,member));
}

void Cfg::ignore(QString key, QObject *reciever, const char *member)
{
    inst.listeners[key].remove(ListenItem(reciever,member));
}

void Cfg::alarmListeners()
{
    bool hasChange=false;
    foreach(QString key,changed)
    {
        hasChange=true;
        foreach(ListenItem item,listeners[key])
        {
            //QMetaObject::invokeMethod(item.first,item.second);
            connect(this,SIGNAL(alarm()),item.first,item.second);
        }
    }
    if(hasChange)
    {
        changed.clear();
        save();
        emit(alarm());
        disconnect(this,SIGNAL(alarm()),0,0);
    }
}

void Cfg::pauseAlarm(bool b)
{
    if(b)
        inst.tick.stop();
    else
    {
        QTimer::singleShot(0,&inst,SLOT(alarmListeners()));
        inst.tick.start();
    }
}
