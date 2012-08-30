#include "ucalltagselectdlg.h"
#include "ui_ucalltagselectdlg.h"
#include "../funclistitem.h"
#include <QSet>
#include <QTimer>

UCallTagSelectDlg::UCallTagSelectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UCallTagSelectDlg)
{
    ui->setupUi(this);
}

UCallTagSelectDlg::~UCallTagSelectDlg()
{
    delete ui;
}

void UCallTagSelectDlg::setFunctions(const QList<UAlgorithmEditor::FunctionInfo>& list)
{
    funcs=list;
    QTimer::singleShot(0,this,SLOT(init()));
}

void UCallTagSelectDlg::init()
{
    ui->moduleSel->clear();
    ui->moduleSel->addItem("-- All --");
    ui->moduleSel->addItem("-- Current --");
    QSet<QString> mods;
    foreach(UAlgorithmEditor::FunctionInfo item,funcs)
    {
        if(item.moduleName!=":/"&&!mods.contains(item.moduleName))
        {
            mods.insert(item.moduleName);
            ui->moduleSel->addItem(item.moduleName);
        }
    }
    ui->moduleSel->setCurrentIndex(0);
}
#include <QDebug>
void UCallTagSelectDlg::on_moduleSel_currentIndexChanged(const QString &ritem)
{
    ui->functionSel->disconnect(this);
    ui->functionSel->clear();
    QString citem=(ritem=="-- Current --")?":/":ritem;
    bool isAll=citem=="-- All --";
    int i=0;
    foreach(UAlgorithmEditor::FunctionInfo item,funcs)
    {
        if(isAll||item.moduleName==citem)
        {
            ui->functionSel->addItem((isAll?item.moduleName+"/":"")+item.functionName,item.hint);
            if(i!=-1&&item.moduleName==selectionModule&&item.functionName==selectionFunction)
            {
                ui->functionSel->setCurrentIndex(i);
                functionSel_currentIndexChanged(i);
                i=-1;
            }else
                i++;
        }
    }
    if(!isAll)
        selectionModule=citem;
    connect(ui->functionSel,SIGNAL(currentIndexChanged(int)),SLOT(functionSel_currentIndexChanged(int)));
}

void UCallTagSelectDlg::functionSel_currentIndexChanged(int index)
{
    QString hint=ui->functionSel->itemData(index).toString();
    ui->hint->setText(hint);
    if(ui->moduleSel->currentText()=="-- All --")
    {
        QString fname=ui->functionSel->itemText(index);
        if(fname.left(3)=="://")
        {
            selectionModule=":/";
            selectionFunction=fname.mid(3);
        }else
        {
            QStringList split=fname.split("/");
            selectionModule=split.first();
            split.pop_front();
            selectionFunction=split.join("/");
        }
    }else
    {
        selectionFunction=ui->functionSel->itemText(index);
    }
}
