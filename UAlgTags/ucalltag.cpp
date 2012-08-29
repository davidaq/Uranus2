#include "ucalltag.h"
#include "../ualgorithmeditor.h"
#include "../funclistitem.h"
#include "ucalltagselectdlg.h"
#include <QMenu>

UCallTag::UCallTag()
{
    setIcon(0,QIcon(":/images/algorithm/function.png"));
    setText(0,"Call :");
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
        dlg.setFunctions(editor->getFunctions());
        if(QDialog::Accepted==dlg.exec())
        {

        }
    }
}
