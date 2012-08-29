#ifndef FUNCLISTITEM_H
#define FUNCLISTITEM_H

#include <QTreeWidgetItem>
#include <QObject>

class UContainerTag;
class FuncListItem : public QObject,public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit FuncListItem(QTreeWidget *parent,int type=0);
    ~FuncListItem();
    void setFunctionName(QString);
    void setHintText(QString);
    void dlgEdit();
    QString getFunctionName();
    void setEditable(bool);
    bool isEditable();
    void setText(const QString& text);
    void setBody(UContainerTag *);
    void createBody();
    UContainerTag *getBody() const;
    QStringList& args();
    void update();
private:
    bool editable;
    QString functionName,hintText;
    QStringList arguments;
    int lockCount;
    QTreeWidget* parentWidget;
    UContainerTag* body;

signals:

private slots:
};

#endif // FUNCLISTITEM_H
