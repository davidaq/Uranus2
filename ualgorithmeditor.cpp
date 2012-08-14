#include "ualgorithmeditor.h"
#include "ui_ualgorithmeditor.h"
#include "funclistitem.h"
#include <QDebug>
#include <QMenu>

UAlgorithmEditor::UAlgorithmEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UAlgorithmEditor)
{
    ui->setupUi(this);
    currentModule=ui->functions->topLevelItem(0);
    importedModule=ui->functions->topLevelItem(1);

    currentModule->setExpanded(true);
    importedModule->setExpanded(true);

    FuncListItem* item=new FuncListItem(ui->functions);
    item->setFunctionName("Main");
    item->setHintText("Entrance to a algorithm");
    item->setIcon(0,QIcon(":/images/algorithm/function.png"));
    currentModule->addChild(item);

    on_functions_itemClicked(item,0);

    setWindowIcon(QIcon(":/images/editor.png"));

    edited=true;
    name="Untitled Algorithm";
    updateTitle();
}

UAlgorithmEditor::~UAlgorithmEditor()
{
    delete ui;
}


void UAlgorithmEditor::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    static QMenu menu;
    menu.clear();
    menu.addAction("   [Insert]")->setEnabled(false);
    QMenu* sub=menu.addMenu("Flow control");
    sub->addAction("Conditional branch");
    sub->addAction("Conditional loop");
    sub->addAction("Traverse");
    menu.addAction("Variable assignment");
    menu.addAction("Function call");
    menu.addAction("Literal or variable");
    menu.addAction("Return");
    menu.addSeparator();
    menu.addAction("Copy");
    menu.addAction("Cut");
    menu.addAction("Paste");
    menu.addSeparator();
    menu.addAction("Delete");

    menu.move(ui->treeView->mapToGlobal(pos));
    menu.show();
}

void UAlgorithmEditor::on_functions_itemClicked(QTreeWidgetItem *rawitem, int )
{
    FuncListItem* item=dynamic_cast<FuncListItem*>(rawitem);
    if(item==0)
    {
        rawitem->setExpanded(!rawitem->isExpanded());
        return;
    }
    if(currentFunction==item)
        return;
    disconnect(ui->hintText,SIGNAL(textChanged()));
    currentFunction=item;
    ui->functions->setCurrentItem(item);
    QString hint=item->toolTip(0);
    updateTitle();
    connect(ui->hintText,SIGNAL(textChanged()),SLOT(on_hintTextChanged()));
    ui->hintText->setPlainText(hint);
}

void UAlgorithmEditor::updateTitle()
{
    setWindowTitle((edited?"*":"")+name+"["+currentFunction->text(0)+"]");
}


void UAlgorithmEditor::on_functions_itemDoubleClicked(QTreeWidgetItem *rawitem, int )
{
    FuncListItem* item=dynamic_cast<FuncListItem*>(rawitem);
    if(item==0)
    {
        rawitem->setExpanded(true);
        return;
    }
    item->edit();
}

void UAlgorithmEditor::on_hintTextChanged()
{
    static bool lock=false;
    if(lock)
        return;
    lock=true;
    QString hint=ui->hintText->toPlainText();
    QTextCursor tcur=ui->hintText->textCursor();
    int tcurpos=tcur.position();
    currentFunction->setHintText(hint);
    hint=hint.replace(QRegExp("(\\%[^%]+\\%)"),"<font color='blue'>\\1</font>");
    ui->hintText->clear();
    ui->hintText->appendHtml(hint);
    tcur.setPosition(tcurpos);
    ui->hintText->setTextCursor(tcur);
    lock=false;
}
