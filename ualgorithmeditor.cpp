#include "ualgorithmeditor.h"
#include "ui_ualgorithmeditor.h"
#include "funclistitem.h"
#include "UAlgTags/ualgtag.h"
#include "UAlgTags/ubranchtag.h"
#include "UAlgTags/ulisttag.h"
#include "UAlgTags/ulisttagitem.h"
#include "cfg.h"
#include <QDebug>
#include <QMenu>
#include <QDir>
#include <QFileDialog>
#include <QLinkedList>

UAlgorithmEditor::UAlgorithmEditor(QWidget *parent, MainWindow *base) :
    QWidget(parent),
    ui(new Ui::UAlgorithmEditor)
{
    ui->setupUi(this);

    this->base = base;

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
    connect(item,SIGNAL(destroyed()),SLOT(editedS()));
    item->createBody();
    item->setFunctionName("Entrance");
    item->setHintText("Start point of this algorithm");
    currentModule->addChild(item);

    on_functions_itemClicked(item,0);

    setWindowIcon(QIcon(":/images/editor.png"));

    name="Untitled";
    editedS();

    QVariant var=Cfg::get("BuiltinDir");
    if(var.isValid())
    {
        QDir dir(var.toString());
        foreach(QString file,dir.entryList(QStringList()<<"*.ulib",QDir::Files))
        {
            import(dir.absoluteFilePath(file),true);
        }
    }

    connect(&fixSelectionDelay,SIGNAL(timeout()),SLOT(fixSelection()));
    fixSelectionDelay.setInterval(700);
    fixSelectionDelayActive = false;
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
    editedS();
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

        QRegExp keyWord("(\\s*)(\\w+)\\s",Qt::CaseInsensitive);
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
    info.moduleName=":/";
    for(int i=0,c=currentModule->childCount();i<c;i++)
    {
        FuncListItem* item=dynamic_cast<FuncListItem*>(currentModule->child(i));
        if(item)
        {
            info.functionName=item->getFunctionName();
            info.args=item->args();
            QString hint;
            hint="<i>"+info.moduleName+"</i> - <b>"+info.functionName+"</b><p>";
            hint+=item->toolTip(0)+"<br/>";
            hint+="<i>Arguments:</i>";
            foreach(QString arg,info.args)
            {
                hint+="<br/>  * "+arg;
            }
            hint+="</p>";
            info.hint=hint;
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
                info.functionName=item->getFunctionName();
                info.args=item->args();
                QString hint;
                hint="<i>"+info.moduleName+"</i> - <b>"+info.functionName+"</b><p>";
                hint+=item->toolTip(0)+"<br/>";
                hint+="<i>Arguments:</i>";
                foreach(QString arg,info.args)
                {
                    hint+="<br/>  * "+arg;
                }
                hint+="</p>";
                info.hint=hint;
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
                info.functionName=item->getFunctionName();
                info.args=item->args();
                QString hint;
                hint="<i>"+info.moduleName+"</i> - <b>"+info.functionName+"</b><p>";
                hint+=item->toolTip(0)+"<br/>";
                hint+="<i>Arguments:</i>";
                foreach(QString arg,info.args)
                {
                    hint+="<br/>  * "+arg;
                }
                hint+="</p>";
                info.hint=hint;
                ret<<info;
            }
        }
    }

    return ret;
}

void UAlgorithmEditor::on_functions_customContextMenuRequested(const QPoint &pos)
{
    static QMenu menu;
    menu.clear();
    QTreeWidgetItem *rawItem = ui->functions->itemAt(pos);
    FuncListItem* item = dynamic_cast<FuncListItem*>(rawItem);
    menu.addAction("Add function",this,SLOT(addFunction()));
    if(item!=0){
        menu.addAction("Delete function",item,SLOT(deleteLater()))->setEnabled(currentFunction!=item);
    }
    menu.move(ui->functions->mapToGlobal(pos));
    menu.show();
}

void UAlgorithmEditor::addFunction(){
    FuncListItem* item=new FuncListItem(ui->functions);
    item->createBody();
    item->setFunctionName("*");
    item->setHintText("A new untitled function");
    currentModule->addChild(item);
    editedS();
    connect(item,SIGNAL(destroyed()),SLOT(editedS()));
}

void writeLine(QFile &fp,QString data,int indent)
{
    while(indent-->0){
        fp.write("\t");
    }
    fp.write(data.toUtf8()+"\n");
}

bool UAlgorithmEditor::save(){
    if(path.isEmpty()){
        path = QFileDialog::getSaveFileName(this,"Save algorithm file",base->getCwd());
        if(path.isEmpty())
            return false;
    }
    QFile fp(path);
    if(fp.open(QFile::WriteOnly)){
        name = QFileInfo(path).fileName();

        for(int i=0,c=currentModule->childCount();i<c;i++){
            FuncListItem * function = dynamic_cast<FuncListItem*>(currentModule->child(i));
            if(function==0)
                continue;
            writeLine(fp,"function "+function->getFunctionName());
            writeLine(fp,"hint "+function->toolTip(0),1);
            foreach(QString arg,function->args()){
                writeLine(fp,"arg "+arg,1);
            }
            function->getBody()->save(fp,1);
        }

        edited = false;
        updateTitle();
    }
    return true;
}

bool UAlgorithmEditor::modified(){
    return edited;
}

void UAlgorithmEditor::editedS(){
    edited = true;
    updateTitle();
}

void UAlgorithmEditor::open(QString path){
    QFile fp(path);
    if(!fp.open(QFile::ReadOnly))
        return;
    this->path = path;
    name = QFileInfo(path).fileName();
    QLinkedList<UAlgTag*> stack;
    QRegExp keyWord("(\\s*)(\\w+)\\s",Qt::CaseInsensitive);
    FuncListItem * function=0,*first=0;
    int functionIndent=0,pIndent,cIndent=0;
    foreach(QTreeWidgetItem* item,currentModule->takeChildren()){
        delete item;
    }
    while(!fp.atEnd()){
        QString line=fp.readLine();
        keyWord.indexIn(line);
        QString key=keyWord.cap(2).toLower();
        line=line.mid(keyWord.cap().length()).trimmed();
        pIndent = cIndent;
        cIndent=keyWord.cap(1).length();
        if(cIndent<functionIndent){
            if(function!=0){
                function->update();
                function=0;
            }
        }else if(cIndent<=pIndent){
            while(pIndent>=cIndent){
                pIndent--;
                if(!stack.isEmpty())
                    stack.pop_back();
            }
        }
        if(key=="function"){
            function=new FuncListItem(ui->functions);
            function->setFunctionName(line);
            function->setEditable(true);
            function->createBody();
            functionIndent=cIndent+1;
            currentModule->addChild(function);
            stack.clear();
            if(first==0)
                first = function;
        }else if(key=="hint"){
            if(cIndent==functionIndent&&function)
                function->setHintText(line);
            else if(!stack.isEmpty()){
                UCommentTag* comment = new UCommentTag;
                comment->setComment(line);
                stack.last()->addChild(comment);
            }
        }else if(key=="arg"){
            if(cIndent==functionIndent&&function){
                function->args()<<line;
            }
        }else if(key=="assign"){
            if(stack.isEmpty())
                continue;
            UAssignTag *assign = new UAssignTag;
            assign->setVarname(line);
            stack.last()->addChild(assign);
            stack.push_back(assign);
        }else if(key=="branch"){
            if(stack.isEmpty())
                continue;
            UBranchTag* branch = new UBranchTag(true);
            stack.last()->addChild(branch);
            stack.push_back(branch);
        }else if(key=="call"){
            if(stack.isEmpty())
                continue;
            UCallTag* call = new UCallTag;
            if(line.left(3)=="://"){
                call->setFunction(":/",line.mid(3));
            }else{
                QStringList f = line.split("/");
                if(f.count()==2){
                    call->setFunction(f.at(0),f.at(1));
                }
            }
            stack.last()->addChild(call);
            stack.push_back(call);
        }else if(key=="conditional"){
            if(stack.isEmpty())
                continue;
            UConditionTag* condition = new UConditionTag(true);
            stack.last()->addChild(condition);
            stack.push_back(condition);
        }else if(key=="do"){
            if(cIndent>functionIndent&&!stack.isEmpty()){
                UContainerTag* container = new UContainerTag;
                stack.last()->addChild(container);
                stack.push_back(container);
            }else if(function!=0)
                stack.push_back(function->getBody());
        }else if(key=="list"){
            if(stack.isEmpty())
                continue;
            UListTag *list = new UListTag;
            stack.last()->addChild(list);
            stack.push_back(list);
        }else if(key=="item"){
            if(stack.isEmpty())
                continue;
            UListTag* list = dynamic_cast<UListTag*>(stack.last());
            if(list!=0){
                UListTagItem* item = new UListTagItem;
                item->setTagName("item");
                list->addChild(item);
                stack.push_back(list);
            }
        }else if(key=="loop"){
            if(stack.isEmpty())
                continue;
            ULoopTag* loop = new ULoopTag(true);
            stack.last()->addChild(loop);
            stack.push_back(loop);
        }else if(key=="traverse"){
            if(stack.isEmpty())
                continue;
            UTraverseTag* traverse = new UTraverseTag(true);
            stack.last()->addChild(traverse);
            stack.push_back(traverse);
        }else if(key=="condition"){
            if(stack.isEmpty())
                continue;
            UArgHook* condition=new UArgHook;
            condition->setIndependent(false);
            condition->setTagName("condition");
            condition->setIcon(0,QIcon(":/images/algorithm/condition.png"));
            condition->setArgValue(line);
            stack.last()->addChild(condition);
            stack.push_back(condition);
        }else if(key=="return"){
            if(stack.isEmpty())
                continue;
            UArgHook* item=new UArgHook;
            item->setIcon(0,QIcon(":/images/algorithm/return.png"));
            item->setTagName("return");
            item->setIndependent(true);
            item->setArgValue(line);
            stack.last()->addChild(item);
            stack.push_back(item);
        }else if(key=="execute"){
            if(stack.isEmpty())
                continue;
            UArgHook* item=new UArgHook;
            item->setIcon(0,QIcon(":/images/algorithm/command.png"));
            item->setTagName("execute");
            item->setIndependent(true);
            item->setArgValue(line);
            stack.last()->addChild(item);
            stack.push_back(item);
        }else if(key=="break"){
            if(stack.isEmpty())
                continue;
            UStaticTag* item=new UStaticTag("break");
            item->setIcon(QIcon(":/images/algorithm/break.png"));
            item->setText("break");
            item->setIndependent(true);
            stack.last()->addChild(item);
            stack.push_back(item);
        }else if(key=="continue"){
            if(stack.isEmpty())
                continue;
            UStaticTag* item=new UStaticTag("continue");
            item->setIcon(QIcon(":/images/algorithm/continue.png"));
            item->setText("break");
            item->setIndependent(true);
            stack.last()->addChild(item);
            stack.push_back(item);
        }else{
            if(stack.isEmpty())
                continue;
            UCallTag* call = dynamic_cast<UCallTag*>(stack.last());
            if(call!=0){
                UArgHook *arg=new UArgHook;
                arg->setIcon(0,QIcon(":/images/algorithm/var.png"));
                arg->setTagName(key);
                arg->setArgValue(line);
                call->addChild(arg);
                stack.last()->addChild(arg);
                stack.push_back(arg);
            }
        }
    }
    if(function!=0){
        function->update();
        on_functions_itemClicked(first,0);
    }
    edited = false;
    updateTitle();
}

void UAlgorithmEditor::fixSelection()
{
    if(fixSelectionDelayActive)
        return;
    QList<QTreeWidgetItem*> list = ui->body->selectedItems();
    if(list.isEmpty())
        return;
    fixSelectionDelay.stop();
    fixSelectionDelayActive = true;
    int depth = 0x7ffffff;
    foreach(QTreeWidgetItem* item,list){
        qDebug()<<"loop1";
        UAlgTag* tag = dynamic_cast<UAlgTag*>(item);
        if(tag!=0){
            if(tag->getDepth()<depth)
                depth = tag->getDepth();
        }
    }
    QSet<UAlgTag*> selected;
    QLinkedList<UAlgTag*> queue;
    foreach(QTreeWidgetItem* item,list){
        qDebug()<<"loop2";
        UAlgTag* tag = dynamic_cast<UAlgTag*>(item);
        if(tag!=0&&!selected.contains(tag)){
            while(tag->getDepth()!=depth){
                qDebug()<<"loop3";
                tag=dynamic_cast<UAlgTag*>(tag->QTreeWidgetItem::parent());
                if(tag==0)
                    break;
            }
            if(tag==0)
                continue;
            queue.push_back(tag);
            while(!queue.isEmpty()){
                qDebug()<<"loop4";
                UAlgTag* ctag = queue.first();
                queue.pop_front();
                selected.insert(ctag);
                for(int i=0,c=ctag->childCount();i<c;i++){
                    tag = dynamic_cast<UAlgTag*>(ctag->child(i));
                    if(tag!=0)
                        queue.push_back(tag);
                }
            }
        }
    }
    ui->body->clearSelection();
    foreach(UAlgTag* item,selected){
        item->setSelected(true);
    }
    qDebug()<<"sel!!";
    fixSelectionDelayActive = false;
}

void UAlgorithmEditor::on_body_itemSelectionChanged()
{
    fixSelectionDelay.stop();
    if(!fixSelectionDelayActive)
        fixSelectionDelay.start();
}
