#ifndef FUNCTION_H
#define FUNCTION_H
#include <QString>
#include <QLinkedList>
#include <QStringList>
#include <QFile>

class StackItem{
public:
    StackItem(QString key,QString line,int indent){
        this->key = key;
        this->line = line;
        this->indent = indent;
        child = 0;
        parent = 0;
        data = 0;
    }
    ~StackItem(){
        if(child){
            foreach(StackItem* item,*child){
                delete item;
            }
            delete child;
        }
    }
    int indent,data;
    QString key;
    QString line;
    QString outBegin;
    QString outEnd;
    QString outSep;
    QString getOut(){
        QString ret=outBegin;
        if(child){
            QStringList list;
            foreach(StackItem* item,*child){
                list.append(item->getOut());
            }
            ret+=list.join(outSep);
        }
        ret+=outEnd;
        return ret;
    }
    StackItem* parent;
    QLinkedList<StackItem*>* child;
    void addChild(StackItem* item){
        if(child==0)
            child = new QLinkedList<StackItem*>;
        child->append(item);
        item->parent=this;
    }
};

class Function
{
private:
    QString mod,name;
    QStringList _args;
    QLinkedList<StackItem*> list;
public:
    Function(QString modname,QString fname);
    void write(QFile &) const;
    void append(StackItem*);
    QStringList& args();
    static QString conv(QString mod,QString name);
};

#endif // FUNCTION_H
