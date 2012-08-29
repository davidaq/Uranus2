#include "ucalltagselectdlg.h"
#include "ui_ucalltagselectdlg.h"
#include "../funclistitem.h"
#include <QSet>

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
    ui->functionSel->clear();
    ui->moduleSel->addItem("-- All --");
    ui->moduleSel->addItem("-- Current --");
    QSet<QString> mods;
    foreach(UAlgorithmEditor::FunctionInfo item,funcs)
    {
        if(item.moduleName!="/"&&!mods.contains(item.moduleName))
        {
            mods.insert(item.moduleName);
            ui->moduleSel->addItem(item.moduleName);
        }
    }
    ui->functionSel->setCurrentIndex(0);
}

void UCallTagSelectDlg::on_moduleSel_currentIndexChanged(const QString &ritem)
{
    ui->functionSel->clear();
    QString citem=(ritem=="-- Current --")?"/":ritem;
    if(citem=="-- All --")
    {
        foreach(UAlgorithmEditor::FunctionInfo item,funcs)
        {
            QString hint;
            hint="<i>"+item.moduleName+"</i> - <b>"+item.functionName+"</b><br/>";
            hint+=item.hint+"<br/>";
            hint+="<i>Arguments:</i>";
            hint+="<ul><li>";
            hint+=item.args.join("</li><li>");
            hint+="</li></ul>";
            if(item.moduleName=="/")
                ui->functionSel->addItem(item.functionName,hint);
            else
                ui->functionSel->addItem(item.moduleName+"."+item.functionName,hint);
        }
    }else
    {
        foreach(UAlgorithmEditor::FunctionInfo item,funcs)
        {
            if(item.moduleName==citem)
                ui->functionSel->addItem(item.functionName);
        }
    }
}

void UCallTagSelectDlg::on_functionSel_currentIndexChanged(int index)
{
    QString hint=ui->functionSel->itemData(index).toString();
    ui->hint->setText(hint);
}
