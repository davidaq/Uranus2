#include "mainwindow.h"
#include <QInputDialog>

void MainWindow::benchMenu_mkdir()
{
    bool ok;
    QString name=QInputDialog::getText(this,"Make Directory","Directory Name",QLineEdit::Normal,"",&ok);
    if(ok&&!name.isEmpty()&&benchModel)
    {
        QModelIndex item;
        if(benchModel->isDir(benchMenuTarget))
        {
            item=benchModel->mkdir(benchMenuTarget,name);
        }else
        {
            item=benchModel->mkdir(benchMenuTarget.parent(),name);
        }

        fileView->setCurrentIndex(item);
    }
}
