#include "configdlg.h"
#include "ui_configdlg.h"
#include "cfg.h"
#include <QFileDialog>

ConfigDlg::ConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDlg)
{
    ui->setupUi(this);
    init(ui->pythonLoc);
    init(ui->consolePath);
}

ConfigDlg::~ConfigDlg()
{
    delete ui;
}

void ConfigDlg::init(QWidget *item)
{
    QString key=item->whatsThis();
    QLineEdit* line=qobject_cast<QLineEdit*>(item);
    if(line!=0)
    {
        QVariant var=Cfg::get(key);
        if(var.isValid())
            line->setText(var.toString());
        connect(line,SIGNAL(textChanged(QString)),SLOT(changed()));
        return;
    }
}

void ConfigDlg::changed()
{
    QLineEdit* line=qobject_cast<QLineEdit*>(sender());
    if(line!=0)
    {
        Cfg::set(line->whatsThis(),line->text());
        return;
    }
}

void ConfigDlg::on_pythonLocOpen_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,"Python location");
    if(!path.isEmpty())
        ui->pythonLoc->setText(path);
}
