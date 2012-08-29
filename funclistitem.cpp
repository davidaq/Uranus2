#include "funclistitem.h"
#include "UAlgTags/ucontainertag.h"
#include <QtGui>
#include <QTimer>

FuncListItem::FuncListItem(QTreeWidget *parent,int type) :
    QTreeWidgetItem((QTreeWidget *)0,type)
{
    lockCount=0;
    parentWidget=parent;
    body=0;
    setIcon(0,QIcon(":/images/algorithm/function.png"));
    editable=true;
}

FuncListItem::~FuncListItem()
{
    if(body)
        delete body;
}

void FuncListItem::setFunctionName(QString name)
{
    functionName=name;
    update();
}

void FuncListItem::setHintText(QString text)
{
    hintText=text;
    setToolTip(0,hintText);
}

void FuncListItem::dlgEdit()
{
    QDialog dlg;
    dlg.setWindowTitle("Function setting");
    dlg.setMaximumWidth(300);
    dlg.setMinimumWidth(300);
    dlg.setMaximumHeight(200);
    dlg.setMinimumHeight(200);
    dlg.resize(300,200);
    QLineEdit funcName;
    funcName.setParent(&dlg);
    funcName.setText(functionName);
    funcName.resize(300,30);
    QPlainTextEdit text(&dlg);
    text.move(0,30);
    text.resize(300,120);
    text.setPlainText(hintText);
    QLabel lbl("Indicate argument with %argument name%",&dlg);
    lbl.move(5,150);
    lbl.setStyleSheet("color:#777");
    QPushButton ok("OK",&dlg);
    ok.move(100,170);
    QPushButton cancel("Cancel",&dlg);
    cancel.move(200,170);

    dlg.connect(&ok,SIGNAL(clicked()),&dlg,SLOT(accept()));
    dlg.connect(&cancel,SIGNAL(clicked()),&dlg,SLOT(reject()));

    if(QDialog::Accepted==dlg.exec())
    {
        setFunctionName(funcName.text());
        setHintText(text.toPlainText());
    }
}

QString FuncListItem::getFunctionName()
{
    return functionName;
}

void FuncListItem::update()
{
    setText(functionName+"("+arguments.join(",")+")");
}

void FuncListItem::setText(const QString &text)
{
    QTreeWidgetItem::setText(0,text);
}

UContainerTag* FuncListItem::getBody() const
{
    return body;
}

void FuncListItem::setBody(UContainerTag *item)
{
    body=item;
}

void FuncListItem::createBody()
{
    body=new UContainerTag;
    body->setText(0,"Action");
    body->setIndependent(false);
}

QStringList& FuncListItem::args()
{
    return arguments;
}

void FuncListItem::setEditable(bool e)
{
    editable=e;
}

bool FuncListItem::isEditable()
{
    return editable;
}
