#include "ualgorithmeditor.h"
#include "ui_ualgorithmeditor.h"

UAlgorithmEditor::UAlgorithmEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UAlgorithmEditor)
{
    ui->setupUi(this);
}

UAlgorithmEditor::~UAlgorithmEditor()
{
    delete ui;
}

