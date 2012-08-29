#ifndef UCALLTAGSELECTDLG_H
#define UCALLTAGSELECTDLG_H

#include "../ualgorithmeditor.h"
#include <QDialog>

namespace Ui {
class UCallTagSelectDlg;
}

class UCallTagSelectDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit UCallTagSelectDlg(QWidget *parent = 0);
    ~UCallTagSelectDlg();
    void setFunctions(const QList<UAlgorithmEditor::FunctionInfo>&);
    
private slots:
    void on_moduleSel_currentIndexChanged(const QString &arg1);

    void on_functionSel_currentIndexChanged(int index);

private:
    QList<UAlgorithmEditor::FunctionInfo> funcs;
    Ui::UCallTagSelectDlg *ui;
};

#endif // UCALLTAGSELECTDLG_H
