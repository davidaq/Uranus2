#include "consoleinput.h"
#include "ui_consoleinput.h"

ConsoleInput::ConsoleInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleInput)
{
    ui->setupUi(this);
}

ConsoleInput::~ConsoleInput()
{
    delete ui;
}
