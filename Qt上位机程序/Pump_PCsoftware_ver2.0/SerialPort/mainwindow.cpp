#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());
            serial.close();
        }
    }
    //设置波特率下拉菜单默认显示第三项
    ui->BaudBox->setCurrentIndex(3);
    //关闭发送按钮的使能
    ui->sendButton->setEnabled(false);
    //关闭 系统开机延时时间设置的文本输入框 的 使能
    ui->lineEdit_SystemStartDelayTime->setEnabled(false);

    qDebug() << tr("界面设定成功！");

    this->setWindowTitle("进样蠕动泵控制系统 ver1.0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//清空接受窗口
void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

//发送数据
void MainWindow::on_sendButton_clicked()
{
    serial->write(ui->textEdit_2->toPlainText().toLatin1());
}

//读取接收到的数据
void MainWindow::Read_Data()
{
    QByteArray buf;
    buf = serial->readAll();
    if(!buf.isEmpty())
    {
        QString str = ui->textEdit->toPlainText();
        str+=tr(buf);
        ui->textEdit->clear();
        ui->textEdit->append(str);
    }
    buf.clear();
}

void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        switch(ui->BitNumBox->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->StopBox->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitNumBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->openButton->setText(tr("关闭串口"));   // *********改变 if语句 的判断条件************
        ui->sendButton->setEnabled(true);

        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));  // *********改变 if语句 的判断条件************
        ui->sendButton->setEnabled(false);
    }
}



void MainWindow::on_pushButton_PumpA_clicked()
{
    ui->textEdit_2->setText("a");
    on_sendButton_clicked();
    Read_Data();
}

void MainWindow::on_pushButton_PumpB_clicked()
{
    ui->textEdit_2->setText("b");
    on_sendButton_clicked();
}

//下面这个槽函数暂时没用上
void MainWindow::on_pushButton_clearLineText2_clicked()
{
    ui->textEdit_2->clear();  //清空串口

}


//修改蠕动泵A的进样时间
void MainWindow::on_lineEdit_SamplingTime_PumpA_returnPressed()
{
    QString SamplingTime_PumpA = ui->lineEdit_SamplingTime_PumpA->text();

    //弹出的对话框结束之后才会向下执行
    if ( QMessageBox::Yes == QMessageBox::question(this,tr("进样时间设置"), "蠕动泵A进样时间设置为：" +SamplingTime_PumpA + "ms ",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes) )
    {
        QMessageBox::information(this, tr("进样时间设置"), tr("设置成功！"));

        //把用户设置好的进样时间通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "PAST," + SamplingTime_PumpA );
        on_sendButton_clicked();
    }
    else
    {
        QMessageBox::information(this, tr("进样时间设置"), tr("取消设置"));
    }

    //ui->textEdit_2->setText("*********");   //测试语句
}


void MainWindow::on_lineEdit_samplingTime_PumpB_returnPressed()
{
    QString SamplingTime_PumpB = ui->lineEdit_samplingTime_PumpB->text();

    if ( QMessageBox::Yes == QMessageBox::question(this,tr("进样时间设置"), "蠕动泵B进样时间设置为：" +SamplingTime_PumpB + "ms ",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes) )
    {
        QMessageBox::information(this, tr("进样时间设置"), tr("设置成功！"));

        //把用户设置好的进样时间通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "PBST," + SamplingTime_PumpB );
        on_sendButton_clicked();
    }
    else
    {
        QMessageBox::information(this, tr("进样时间设置"), tr("取消设置"));
    }
}

void MainWindow::on_lineEdit_leftTime_PumpA_returnPressed()
{
    QString LeftTime_PumpA = ui->lineEdit_leftTime_PumpA->text();

    //弹出的对话框结束之后才会向下执行
    if ( QMessageBox::Yes == QMessageBox::question(this,tr("剩余延时时间设置"), "蠕动泵A剩余延时时间设置为：" +LeftTime_PumpA + "s ",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes) )
    {
        QMessageBox::information(this, tr("剩余延时时间设置"), tr("设置成功！"));

        //通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "PALT," + LeftTime_PumpA );
        on_sendButton_clicked();
    }
    else
    {
        QMessageBox::information(this, tr("剩余延时时间设置"), tr("取消设置"));
    }
}

void MainWindow::on_lineEdit_leftTime_PumpB_returnPressed()
{
    QString LeftTime_PumpB = ui->lineEdit_leftTime_PumpB->text();

    if ( QMessageBox::Yes == QMessageBox::question(this,tr("剩余延时时间设置"), "蠕动泵B剩余延时时间设置为：" +LeftTime_PumpB + "s ",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes) )
    {
        QMessageBox::information(this, tr("剩余延时时间设置"), tr("设置成功！"));

        //通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "PBLT," + LeftTime_PumpB );
        on_sendButton_clicked();
    }
    else
    {
        QMessageBox::information(this, tr("剩余延时时间设置"), tr("取消设置"));
    }
}

void MainWindow::on_lineEdit_setFrequency_PumpA_returnPressed()
{
    QString Frequency_PumpA = ui->lineEdit_setFrequency_PumpA->text();

    //弹出的对话框结束之后才会向下执行
    if ( QMessageBox::Yes == QMessageBox::question(this,tr("脉冲信号频率设置"), "蠕动泵A脉冲信号的频率设置为：" +Frequency_PumpA + "Hz ",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes) )
    {
        QMessageBox::information(this, tr("脉冲信号频率设置"), tr("设置成功！"));

        //通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "PAspeed," + Frequency_PumpA );
        on_sendButton_clicked();
    }
    else
    {
        QMessageBox::information(this, tr("脉冲信号频率设置"), tr("取消设置"));
    }
}

void MainWindow::on_lineEdit_setFrequency_PumpB_returnPressed()
{
    QString Frequency_PumpB = ui->lineEdit_setFrequency_PumpB->text();

    //弹出的对话框结束之后才会向下执行
    if ( QMessageBox::Yes == QMessageBox::question(this,tr("脉冲信号频率设置"), "蠕动泵B脉冲信号的频率设置为：" +Frequency_PumpB + "Hz ",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes) )
    {
        QMessageBox::information(this, tr("脉冲信号频率设置"), tr("设置成功！"));

        //通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "PBspeed," + Frequency_PumpB );
        on_sendButton_clicked();
    }
    else
    {
        QMessageBox::information(this, tr("脉冲信号频率设置"), tr("取消设置"));
    }
}

//蠕动泵进样系统工作时序
void MainWindow::on_lineEdit_commandOrder_returnPressed()
{
    QString CommandOrder = ui->lineEdit_commandOrder->text();

    //弹出的对话框结束之后才会向下执行
    if ( QMessageBox::Yes == QMessageBox::question(this,tr("工作时序设置"), "蠕动泵工作时序设置为：" +CommandOrder + " ",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes) )
    {
        QMessageBox::information(this, tr("工作时序设置"), tr("设置成功！"));

        //通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( CommandOrder );
        on_sendButton_clicked();
    }
    else
    {
        QMessageBox::information(this, tr("工作时序设置"), tr("取消设置"));
    }
}

void MainWindow::on_checkBox_startDelay_clicked()  // 启动开机延时
{
    if( ui->checkBox_startDelay->isChecked() == true )
    {
        //通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "SysStartDelay,1" );
        on_sendButton_clicked();
        //开启 系统开机延时时间设置的文本输入框 的 使能
        ui->lineEdit_SystemStartDelayTime->setEnabled(true);
    }
    else
    {
        //通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "SysStartDelay,0" );
        on_sendButton_clicked();

        ui->lineEdit_SystemStartDelayTime->clear();
        //关闭 系统开机延时时间设置的文本输入框 的 使能
        ui->lineEdit_SystemStartDelayTime->setEnabled(false);

    }
}

void MainWindow::on_lineEdit_SystemStartDelayTime_returnPressed()
{
    QString SystemStartDelayTime = ui->lineEdit_SystemStartDelayTime->text();

    if ( QMessageBox::Yes == QMessageBox::question(this,tr("启动延时设置"), "系统启动延时时间设置为：" +SystemStartDelayTime + "s ",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes) )
    {
        QMessageBox::information(this, tr("启动延时设置"), tr("设置成功！"));

        //把用户设置好的进样时间通过串口发送给下位机（arduino）
        ui->textEdit_2->setText( "SSDT," + SystemStartDelayTime );
        on_sendButton_clicked();
    }
    else
    {
        QMessageBox::information(this, tr("启动延时设置"), tr("取消设置"));
    }
}
