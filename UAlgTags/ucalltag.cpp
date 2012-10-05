#include "ucalltag.h"
#include "../ualgorithmeditor.h"
#include "../funclistitem.h"
#include "ucalltagselectdlg.h"
#include <QMenu>

UCallTag::UCallTag()
{
    setIcon(0,QIcon(":/images/algorithm/function.png"));
    setText(0,"Call <undefined>");
}

void UCallTag::menu(QMenu &menu)
{
    menu.setDefaultAction(
                menu.addAction("Select function",this,SLOT(selectFunction()))
        );
}

QString UCallTag::tagName() const
{
    return "call";
}

void UCallTag::selectFunction()
{
    UAlgorithmEditor* editor=dynamic_cast<UAlgorithmEditor*>(treeWidget()->parentWidget());
    if(editor)
    {
        UCallTagSelectDlg dlg;
        QList<UAlgorithmEditor::FunctionInfo> info=editor->getFunctions();
        dlg.selectionModule=moduleName;
        dlg.selectionFunction=functionName;
        dlg.setFunctions(info);
        if(QDialog::Accepted==dlg.exec())
        {
            if(setFunction(dlg.selectionModule,dlg.selectionFunction))
            {
                foreach(UAlgorithmEditor::FunctionInfo f,info)
                {
                    if(f.moduleName==dlg.selectionModule&&f.functionName==dlg.selectionFunction)
                    {
                        setArgs(f.args);
                        break;
                    }
                }
            }
        }
    }
}

bool UCallTag::setFunction(const QString& mod,const QString& func)
{
    if(mod==moduleName&&func==functionName)
        return false;
    functionName=func;
    moduleName=mod;
    QString name;
    if(mod!=":/")
        name=mod+"/";
    name+=func;
    setText(0,"Call "+name);
    return true;
}

void UCallTag::setArgs(QStringList list)
{
    if(list!=args)
    {
        foreach(QTreeWidgetItem* item,takeChildren())
        {
            delete item;
        }
        foreach(QString item,list)
        {
            UArgHook *arg=new UArgHook;
            arg->setIcon(0,QIcon(":/images/algorithm/var.png"));
            arg->setTagName(item);
            addChild(arg);
        }
        args=list;
    }
    if(!list.isEmpty())
        setText(0,text(0)+" with arguments:");
}

void UCallTag::save(QFile &fp, int depth) const
{
    writeLine(fp,"call "+moduleName+"/"+functionName,depth);
    UAlgTag::save(fp,depth);
}
