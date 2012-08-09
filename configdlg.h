#ifndef CONFIGDLG_H
#define CONFIGDLG_H

#include <QDialog>

namespace Ui {
class ConfigDlg;
}

class ConfigDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDlg(QWidget *parent = 0);
    ~ConfigDlg();
    
private:
    Ui::ConfigDlg *ui;
    void init(QWidget*);
private slots:
    void changed();
    void on_pythonLocOpen_clicked();
};

#endif // CONFIGDLG_H
