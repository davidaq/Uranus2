#include <QtCore/QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QSet>
#include <QLinkedList>
#include <QTextCodec>
#include <QDebug>
#include "function.h"
#define FILE_HEAD "be25a38b316d72acf2e6b5da9169d0df"

QString readLine(QFile &fp){
    return QString::fromUtf8(fp.readLine());
}

void compile(QString path,QStringList& import,QFile &out){
    QFile fp(path);
    if(fp.open(QFile::ReadOnly)){
        QDir dir(QFileInfo(fp).absoluteDir());
        QString modName = QFileInfo(fp).baseName();
        if(readLine(fp).trimmed()!=FILE_HEAD){
            qDebug("Indicated file is not a algorithm file of Uranus2");
            exit(0);
        }
        int pIndent,cIndent=0,functionIndent=0;
        Function* function=0;
        QRegExp keyWord("(\\s*)(\\w+)\\s",Qt::CaseInsensitive);
        QRegExp argHook("(<.+>)?(.+)?");
        QLinkedList<StackItem*> stack;
        while(!fp.atEnd()){
            QString line=QString::fromUtf8(fp.readLine());
            keyWord.indexIn(line);
            QString key=keyWord.cap(2).toLower();
            line=line.mid(keyWord.cap().length()).trimmed();
            pIndent = cIndent;
            cIndent=keyWord.cap(1).length();
            if(cIndent<functionIndent){
                if(function!=0){
                    function->write(out);
                    function=0;
                }
            }else if(cIndent<=pIndent){
                while(pIndent>=cIndent){
                    pIndent--;
                    if(!stack.isEmpty()){
                        stack.pop_back();
                    }
                }
            }
            StackItem* parent = (stack.isEmpty())?0:stack.back(),
                    *self=new StackItem(key,line,cIndent);
            stack.push_back(self);

            if(key=="import"){
                import.append(dir.absoluteFilePath(line));
            }else if(key=="function"){
                function=new Function(modName,line);
                functionIndent=cIndent+1;
            }else if(key=="hint"){
                if(function!=0){
                    if(parent->key=="function")
                        self->indent++;
                    self->outBegin="#"+line;
                    function->append(self);
                }
            }else if(key=="arg"){
                if(parent->key=="function"&&function){
                    function->args()<<line;
                }
            }else if(key=="assign"){
                self->outBegin=line+"=";
                function->append(self);
            }else if(key=="value"){
                if(parent->key=="assign"){
                    argHook.indexIn(line);
                    if(argHook.cap(1)=="<list>"){
                    }else if(argHook.cap(1)=="<function>"){
                    }else if(argHook.cap(1)=="<variable>"){
                        self->outBegin=argHook.cap(2).trimmed();
                    }else{
                        self->outBegin=line.trimmed();
                    }
                    parent->addChild(self);
                }
            }else if(key=="branch"){
                self->outBegin="if True:";
                function->append(self);
            }else if(key=="call"){
                QString fname;
                if(line.left(3)=="://"){
                    fname=Function::conv(modName,line.mid(3));
                }else{
                    QStringList f = line.split("/");
                    if(f.count()==2){
                        fname=Function::conv(f.at(0),f.at(1));
                    }
                }
                self->outBegin=fname+"(";
                self->outSep=",";
                self->outEnd=")";
                if(parent->key=="do"){
                    function->append(self);
                }else{
                    parent->addChild(self);
                }
            }else if(key=="conditional"){
                self->outBegin="if True:";
                function->append(self);
            }else if(key=="do"){
            }else if(key=="list"){
                self->outBegin="[";
                self->outSep=",";
                self->outEnd="]";
                parent->addChild(self);
            }else if(key=="item"){
                argHook.indexIn(line);
                if(argHook.cap(1)=="<list>"){
                }else if(argHook.cap(1)=="<function>"){
                }else if(argHook.cap(1)=="<variable>"){
                    self->outBegin=argHook.cap(2).trimmed();
                }else{
                    self->outBegin=line.trimmed();
                }
                parent->addChild(self);
            }else if(key=="loop"){
                self->outBegin="if True:";
                function->append(self);
            }else if(key=="traverseloop"){
                self->outBegin="if True:";
                function->append(self);
            }else if(key=="traverse"){
                self->outBegin="for traverse in ";
                self->outEnd=":";
                function->append(self);
            }else if(key=="condition"){
                self->outEnd=":";
                if(parent->key=="branch"){
                    if(parent->data==0){
                        self->outBegin="if ";
                    }else{
                        self->outBegin="elif ";
                    }
                    parent->data=1;
                }else if(parent->key=="loop"){
                    self->outBegin="while ";
                }
                argHook.indexIn(line);
                if(argHook.cap(1)=="<list>"){
                }else if(argHook.cap(1)=="<function>"){
                }else if(argHook.cap(1)=="<variable>"){
                    self->outBegin+=argHook.cap(2).trimmed();
                }else{
                    self->outBegin+=line.trimmed();
                }
                function->append(self);
            }else if(key=="return"){
                self->outBegin="return ";
                argHook.indexIn(line);
                if(argHook.cap(1)=="<list>"){
                }else if(argHook.cap(1)=="<function>"){
                }else if(argHook.cap(1)=="<variable>"){
                    self->outBegin+=argHook.cap(2).trimmed();
                }else{
                    self->outBegin+=line.trimmed();
                }
                function->append(self);
            }else if(key=="execute"){
                self->outBegin="exec(";
                self->outEnd=")";
                argHook.indexIn(line);
                if(argHook.cap(1)=="<list>"){
                }else if(argHook.cap(1)=="<function>"){
                }else if(argHook.cap(1)=="<variable>"){
                    self->outBegin+=argHook.cap(2).trimmed();
                }else{
                    self->outBegin+=line.trimmed();
                }
                function->append(self);
            }else if(key=="break"){
                self->outBegin = "break";
                function->append(self);
            }else if(key=="continue"){
                self->outBegin = "continue";
                function->append(self);
            }else{
                if(parent->key=="call"){
                    argHook.indexIn(line);
                    if(argHook.cap(1)=="<list>"){
                    }else if(argHook.cap(1)=="<function>"){
                    }else if(argHook.cap(1)=="<variable>"){
                        self->outBegin+=argHook.cap(2).trimmed();
                    }else{
                        self->outBegin+=line.trimmed();
                    }
                    parent->addChild(self);
                }
            }
        }
        if(function!=0)
            function->write(out);
    }else{
        qDebug()<<"Can not open file "+path;
    }
}

int main(int argc, char *argv[])
{
    if(argc<2){
        qDebug("Please indicate a file");
        exit(0);
    }
    QStringList files;
    QSet<QString> compiled;
    QString file=QTextCodec::codecForLocale()->toUnicode(argv[1]);
    files<<file;
    QFile out(file+".py");
    if(out.open(QFile::WriteOnly)){
        while(!files.isEmpty()){
            QString path = files.first();
            if(compiled.contains(path))
                continue;
            compiled.insert(path);
            compile(path,files,out);
            files.pop_front();
        }
        out.write((Function::conv(QFileInfo(file).baseName(),"Entrance")+"()").toUtf8());
    }
}
