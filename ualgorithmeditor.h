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
    
private slots:
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void on_functions_itemClicked(QTreeWidgetItem *item, int column);

    void on_functions_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_hintTextChanged();

private:
    Ui::UAlgorithmEditor *ui;
    QTreeWidgetItem *currentModule,*importedModule;
    FuncListItem* currentFunction;
    bool edited;
    QString name;
    void updateTitle();
    void import(QString,bool builtin=false);
};

#endif // UALGORITHMEDITOR_H
