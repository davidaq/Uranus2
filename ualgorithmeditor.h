#ifndef UALGORITHMEDITOR_H
#define UALGORITHMEDITOR_H

#include "udocument.h"
#include "mainwindow.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class UAlgorithmEditor;
}

#define FILE_HEAD "be25a38b316d72acf2e6b5da9169d0df"

class QTreeWidgetItem;
class FuncListItem;
class UAlgorithmEditor : public QWidget,public UDocument
{
    Q_OBJECT
    
public:
    explicit UAlgorithmEditor(QWidget *parent = 0,MainWindow* base = 0);
    ~UAlgorithmEditor();

    struct FunctionInfo{
        QString functionName;
        QStringList args;
        QString hint;
        QString moduleName;
    };

    QList<FunctionInfo> getFunctions();
    virtual bool save();
    virtual bool modified();
    void open(QString);
//    virtual bool execute();
//    virtual bool executable();
public slots:
    void editedS();
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
    void on_functions_customContextMenuRequested(const QPoint &pos);
    void addFunction();
    void fixSelection();
    void importModule();

    void on_body_itemSelectionChanged();

private:
    MainWindow* base;
    Ui::UAlgorithmEditor *ui;
    QTreeWidgetItem *builtinModule,*currentModule,*importedModule,*currentBody;
    FuncListItem* currentFunction;
    bool edited;
    QString name,path;
    void updateTitle();
    void import(QString,bool builtin=false);
    QTimer fixSelectionDelay;
    bool fixSelectionDelayActive;
};

#endif // UALGORITHMEDITOR_H
