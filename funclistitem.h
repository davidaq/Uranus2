#ifndef FUNCLISTITEM_H
#define FUNCLISTITEM_H

#include <QTreeWidgetItem>
#include <QObject>

class FuncListItem : public QObject,public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit FuncListItem(QTreeWidget *parent,int type=0);
    void setFunctionName(QString);
    void setHintText(QString);
    void dlgEdit();
    QString getFunctionName();
    void edit();
    void setText(const QString& text);
private:
    QString functionName,hintText;
    QStringList arguments;
    int lockCount;
    QTreeWidget* parentWidget;
    void lock();
    void unlock();
    bool isLocked();
    
signals:
    
private slots:
    void nameChanged();
};

#endif // FUNCLISTITEM_H
