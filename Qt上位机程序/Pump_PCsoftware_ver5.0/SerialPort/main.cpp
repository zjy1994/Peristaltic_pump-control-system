#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LoginDialog dlg;
    if( dlg.exec() == QDialog::Accepted )
    {
        w.show();
        QMessageBox::information(NULL, "系统操作说明", "使用该系统时，请您第一步先点击左下方的 ‘打开串口’ 按钮，方能进行后续的设置与操作！");

        return a.exec();
    }
    else return 0;
}
