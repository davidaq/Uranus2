#ifndef UALGORITHMEDITOR_H
#define UALGORITHMEDITOR_H

#include "udocument.h"
#include <QWidget>

namespace Ui {
class UAlgorithmEditor;
}

class UAlgorithmEditor : public QWidget,public UDocument
{
    Q_OBJECT
    
public:
    explicit UAlgorithmEditor(QWidget *parent = 0);
    ~UAlgorithmEditor();
    
private:
    Ui::UAlgorithmEditor *ui;
};

#endif // UALGORITHMEDITOR_H
