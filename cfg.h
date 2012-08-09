#ifndef CFG_H
#define CFG_H

#include <QVariant>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QPair>
#include <QTimer>

class QTimer;
class Cfg:public QObject
{
    Q_OBJECT
private:
    Cfg();
    ~Cfg();
    static Cfg inst;

    QMap<QString,QVariant> table;
    typedef QPair<QObject*,const char* > ListenItem;
    QMap<QString,QSet< ListenItem > > listeners;
    QSet<QString> changed;
    QTimer tick;
private slots:
    void alarmListeners();
    void save();
signals:
    void alarm();
public:
    static QVariant get(QString);
    static void set(QString,QVariant);
    static void listen(QString key,QObject* reciever,const char* member);
    static void ignore(QString key,QObject* reciever,const char* member);
    static void pauseAlarm(bool);
};

#endif // CFG_H
