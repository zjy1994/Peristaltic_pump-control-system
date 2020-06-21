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
        return a.exec();
    }
    else return 0;
}
