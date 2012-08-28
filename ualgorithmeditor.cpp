#include "ualgorithmeditor.h"
#include "ui_ualgorithmeditor.h"
#include "funclistitem.h"
#include "UAlgTags/ualgtag.h"
#include "cfg.h"
#include <QDebug>
#include <QMenu>
#include <QDir>

UAlgorithmEditor::UAlgorithmEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UAlgorithmEditor)
{
    ui->setupUi(this);
    currentModule=ui->functions->topLevelItem(0);
    importedModule=ui->functions->topLevelItem(1);
    ui->functions->topLevelItem(2)->setExpanded(true);

    currentModule->setExpanded(true);
    importedModule->setExpanded(true);

    UFunctionTag* funcTag=new UFunctionTag;
    ui->body->addTopLevelItem(funcTag);

    FuncListItem* item=new FuncListItem(ui->functions);
    item->setFunctionName("Entrance");
    item->setHintText("Start point of this algorithm");
    currentModule->addChild(item);

    on_functions_itemClicked(item,0);

    setWindowIcon(QIcon(":/images/editor.png"));

    edited=true;
    name="Untitled";
    updateTitle();

    QVariant var=Cfg::get("BuiltinDir");
    if(var.isValid())
    {
        QDir dir(var.toString());
        foreach(QString file,dir.entryList(QStringList()<<"*.ulib",QDir::Files))
        {
            import(dir.absoluteFilePath(file),true);
        }
    }

}

UAlgorithmEditor::~UAlgorithmEditor()
{
    delete ui;
}


void UAlgorithmEditor::on_body_customContextMenuRequested(const QPoint &pos)
{
    UAlgTag* tag=dynamic_cast<UAlgTag*>(ui->body->itemAt(pos));
    static QMenu menu;
    menu.clear();
    menu.move(ui->body->mapToGlobal(pos));
    if(tag!=0)
    {
        tag->menu(menu);
    }else{
        menu.addAction("   [Insert]")->setEnabled(false);
        QMenu* sub=menu.addMenu("Flow control");
        sub->addAction("Conditional branch");
        sub->addAction("Conditional loop");
        sub->addAction("Traverse");
        menu.addAction("List");
        menu.addAction("Return");
        menu.addSeparator();
        menu.addAction("Copy");
        menu.addAction("Cut");
        menu.addAction("Paste");
        menu.addSeparator();
        menu.addAction("Delete");
    }
    if(!menu.isEmpty())
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
    currentFunction=item;
    ui->functions->setCurrentItem(item);
    updateTitle();
    ui->body->setEnabled(item->isEditable());
    ui->body->setCurrentItem(0);
    UFunctionTag* funcTag=dynamic_cast<UFunctionTag*>(ui->body->topLevelItem(0));
    if(funcTag)
    {
        funcTag->setName(item->getFunctionName());
        funcTag->setHint(item->toolTip(0));
        funcTag->setArgs(item->args());
    }
}

void UAlgorithmEditor::updateTitle()
{
    setWindowTitle((edited?"*":"")+name);
}


void UAlgorithmEditor::on_functions_itemDoubleClicked(QTreeWidgetItem *rawitem, int )
{
    FuncListItem* item=dynamic_cast<FuncListItem*>(rawitem);
    if(item==0)
    {
        rawitem->setExpanded(true);
        return;
    }
}

void UAlgorithmEditor::import(QString path,bool)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly)){
        QFileInfo finfo(file);
        QTreeWidgetItem *modu=new QTreeWidgetItem;
        modu->setText(0,finfo.baseName());
        modu->setIcon(0,QIcon(":/images/algorithm/command.png"));
        ui->functions->topLevelItem(2)->addChild(modu);

        QRegExp keyWord("(\\s*)(\\w+) ",Qt::CaseInsensitive);
        FuncListItem* funcItem=0;
        int pIndent=0;
        while(!file.atEnd())
        {
            QString line=file.readLine();
            keyWord.indexIn(line);
            QString key=keyWord.cap(2).toLower();
            line=line.mid(keyWord.cap().length()).trimmed();
            int cIndent=keyWord.cap(1).length();
            if(cIndent<=pIndent)
            {
                if(funcItem!=0)
                {
                    funcItem->update();
                    funcItem=0;
                }
            }
            if(key=="function")
            {
                funcItem=new FuncListItem(ui->functions);
                funcItem->setFunctionName(line);
                funcItem->setEditable(false);
                pIndent=cIndent;
                modu->addChild(funcItem);
            }else if(key=="hint")
            {
                if(funcItem)
                    funcItem->setHintText(line);
                else
                    modu->setToolTip(0,line);
            }else if(key=="arg")
            {
                if(funcItem)
                {
                    funcItem->args()<<line;
                }
            }
        }
    }
}
