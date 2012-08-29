#ifndef UALGORITHMEDITOR_H
#define UALGORITHMEDITOR_H

#include "udocument.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class UAlgorithmEditor;
}

class QTreeWidgetItem;
class FuncListItem;
class UAlgorithmEditor : public QWidget,public UDocument
{
    Q_OBJECT
    
public:
    explicit UAlgorithmEditor(QWidget *parent = 0);
    ~UAlgorithmEditor();

    struct FunctionInfo{
        QString functionName;
        QStringList args;
        QString hint;
        QString moduleName;
    };

    QList<FunctionInfo> getFunctions();
    
private slots:
    void on_body_customContextMenuRequested(const QPoint &pos);
    void on_functions_itemClicked(QTreeWidgetItem *item, int column);
    void on_functions_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_body_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_body_itemExpanded(QTreeWidgetItem *item);
    void on_body_itemCollapsed(QTreeWidgetItem *item);
    void funcNameChanged(QString);
    void funcArgsChanged(QStringList);
    void funcHintChanged(QString);
private:
    Ui::UAlgorithmEditor *ui;
    QTreeWidgetItem *builtinModule,*currentModule,*importedModule,*currentBody;
    FuncListItem* currentFunction;
    bool edited;
    QString name;
    void updateTitle();
    void import(QString,bool builtin=false);
};

#endif // UALGORITHMEDITOR_H
