#include <QtCore/QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QSet>
#define FILE_HEAD "be25a38b316d72acf2e6b5da9169d0df"

QString readLine(const QFile &fp){
    return QString::fromUtf8(fp.readLine());
}

void compile(QString path,QStringList& import){
    QFile fp(path);
    if(fp.open(QFile::ReadOnly)){
        if(readLine(fp).trimmed()!=FILE_HEAD){
            qDebug("Indicated file is not a algorithm file of Uranus2");
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc<2){
        qDebug("Please indicate a file");
        exit(0);
    }
    QStringList files;
    QSet compiled;
    files<<argv[1];
    while(!files.isEmpty()){
        QString path = files.first();
        if(compiled.contains(path))
            continue;
        compiled.insert(path);
        compile(import);
        files.pop_front();
    }
}
