#include "function.h"

Function::Function(QString modname,QString fname)
{
    mod=modname;
    name=fname;
}

void Function::append(StackItem *item)
{
    list.append(item);
}

void Function::write(QFile &fp) const
{
    QString head="def "+conv(mod,name)+"("+_args.join(",")+"):\n";
    fp.write(head.toUtf8());
    foreach(StackItem* item,list){
        QString out;
        for(int i=0;i<item->indent;i++){
            out+="\t";
        }
        out+=item->getOut()+"\n";
        fp.write(out.toUtf8());
        delete item;
    }
}

QStringList& Function::args()
{
    return _args;
}

QString encode(QString src)
{
    return src.toAscii().toBase64().replace("=","__");
}

QString Function::conv(QString mod, QString name)
{
    return encode(mod)+"_"+encode(name);
}
