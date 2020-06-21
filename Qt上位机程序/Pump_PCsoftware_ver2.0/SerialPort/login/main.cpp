#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"
#include <QSplashScreen>
#include <QPixmap>
#include <QDateTime>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //LoginDialog dlg;

    //if( dlg.exec() == QDialog::Accepted )
    //{
        QPixmap pixmap("ai.png");
        QSplashScreen screen(pixmap);
        screen.show();
        screen.showMessage("love", Qt::AlignCenter, Qt::red);

        QDateTime n = QDateTime::currentDateTime();
        QDateTime now;
        do{
            now = QDateTime::currentDateTime();
        }while( n.secsTo(now) <=  5 );

        w.show();

        screen.finish(&w);

        return a.exec();
    //}
    //else return 0;
}
