#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

#include <QWidget>

namespace Ui {
class ConsoleInput;
}

class ConsoleInput : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConsoleInput(QWidget *parent = 0);
    ~ConsoleInput();
    
private:
    Ui::ConsoleInput *ui;
};

#endif // CONSOLEINPUT_H
