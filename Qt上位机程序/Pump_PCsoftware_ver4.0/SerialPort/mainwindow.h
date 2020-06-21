#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_clearButton_clicked();

    void on_sendButton_clicked();

    void on_openButton_clicked();

    void Read_Data();

    void timerUpdate();

    void on_pushButton_PumpA_clicked();

    void on_pushButton_PumpB_clicked();

    void on_lineEdit_SamplingTime_PumpA_returnPressed();

    void on_pushButton_clearLineText2_clicked();

    void on_lineEdit_samplingTime_PumpB_returnPressed();

    void on_lineEdit_leftTime_PumpA_returnPressed();

    void on_lineEdit_leftTime_PumpB_returnPressed();

    void on_lineEdit_setFrequency_PumpA_returnPressed();

    void on_lineEdit_setFrequency_PumpB_returnPressed();

    void on_lineEdit_commandOrder_returnPressed();

    void on_checkBox_startDelay_clicked();

    void on_lineEdit_SystemStartDelayTime_returnPressed();

    void on_pushButton_searchDevice_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
