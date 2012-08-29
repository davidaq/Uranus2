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

    currentBody=0;

    currentModule=ui->functions->topLevelItem(0);
    importedModule=ui->functions->topLevelItem(1);
    builtinModule=ui->functions->topLevelItem(2);

    builtinModule->setExpanded(true);
    currentModule->setExpanded(true);
    importedModule->setExpanded(true);

    UFunctionTag* funcTag=new UFunctionTag;
    connect(funcTag,SIGNAL(nameChanged(QString)),SLOT(funcNameChanged(QString)));
    connect(funcTag,SIGNAL(argsChanged(QStringList)),SLOT(funcArgsChanged(QStringList)));
    connect(funcTag,SIGNAL(hintChanged(QString)),SLOT(funcHintChanged(QString)));
    ui->body->addTopLevelItem(funcTag);

    FuncListItem* item=new FuncListItem(ui->functions);
    item->createBody();
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
        tag->callMenu(menu);
    }else{
    }
    if(!menu.isEmpty())
        menu.show();
}

void UAlgorithmEditor::on_body_itemDoubleClicked(QTreeWidgetItem *item, int)
{
    UAlgTag* tag=dynamic_cast<UAlgTag*>(item);
    tag->trigger();
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
    ui->body->takeTopLevelItem(1);
    ui->functions->setCurrentItem(item);
    if(item->getBody())
    {
        ui->body->addTopLevelItem(item->getBody());
    }
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
    ui->body->expandAll();
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
        modu->setIcon(0,QIcon(":/images/algorithm/lib.png"));
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

void UAlgorithmEditor::funcArgsChanged(QStringList list)
{
    QStringList &args=currentFunction->args();
    args=list;
    currentFunction->update();
}

void UAlgorithmEditor::funcNameChanged(QString name)
{
    currentFunction->setFunctionName(name);
}

void UAlgorithmEditor::funcHintChanged(QString text)
{
    currentFunction->setHintText(text);
}

void UAlgorithmEditor::on_body_itemExpanded(QTreeWidgetItem *item)
{
    UAlgTag* tag=dynamic_cast<UAlgTag*>(item);
    if(tag)
        tag->expanded(true);
}

void UAlgorithmEditor::on_body_itemCollapsed(QTreeWidgetItem *item)
{
    UAlgTag* tag=dynamic_cast<UAlgTag*>(item);
    if(tag)
        tag->expanded(false);
}

QList<UAlgorithmEditor::FunctionInfo> UAlgorithmEditor::getFunctions()
{
    QList<FunctionInfo> ret;
    FunctionInfo info;
    info.moduleName="/";
    for(int i=0,c=currentModule->childCount();i<c;i++)
    {
        FuncListItem* item=dynamic_cast<FuncListItem*>(currentModule->child(i));
        if(item)
        {
            info.hint=item->toolTip(0);
            info.functionName=item->getFunctionName();
            info.args=item->args();
            ret<<info;
        }
    }
    for(int i=0,c=importedModule->childCount();i<c;i++)
    {
        QTreeWidgetItem* mod=importedModule->child(i);
        info.moduleName=mod->text(0);
        for(int j=0,cc=mod->childCount();j<cc;j++)
        {
            FuncListItem* item=dynamic_cast<FuncListItem*>(mod->child(j));
            if(item)
            {
                info.hint=item->toolTip(0);
                info.functionName=item->getFunctionName();
                info.args=item->args();
                ret<<info;
            }
        }
    }
    for(int i=0,c=builtinModule->childCount();i<c;i++)
    {
        QTreeWidgetItem* mod=builtinModule->child(i);
        info.moduleName=mod->text(0);
        for(int j=0,cc=mod->childCount();j<cc;j++)
        {
            FuncListItem* item=dynamic_cast<FuncListItem*>(mod->child(j));
            if(item)
            {
                info.hint=item->toolTip(0);
                info.functionName=item->getFunctionName();
                info.args=item->args();
                ret<<info;
            }
        }
    }

    return ret;
}
