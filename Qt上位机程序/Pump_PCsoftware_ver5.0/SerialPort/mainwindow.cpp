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
    ui->lineEdit_commandOrder->setEnabled(false);
    ui->lineEdit_leftTime_PumpA->setEnabled(false);
    ui->lineEdit_leftTime_PumpB->setEnabled(false);
    ui->lineEdit_SamplingTime_PumpA->setEnabled(false);
    ui->lineEdit_samplingTime_PumpB->setEnabled(false);
    ui->lineEdit_setFrequency_PumpA->setEnabled(false);
    ui->lineEdit_setFrequency_PumpB->setEnabled(false);
    ui->checkBox_startDelay->setEnabled(false);
    ui->pushButton_PumpA->setEnabled(false);
    ui->pushButton_PumpB->setEnabled(false);
    //修改按钮的样式
    ui->pushButton_PumpA->setStyleSheet("QPushButton{background-color:gray;\
                                        color:white; border-radius:5px; border:2px groove gray;\
                                        border-style:outset;}"
                                        "QPushButton:hover{background-color:white;color:black;}"
                                        "QPushButton:pressed{background-color:rgb(85,170,255);\
                                                 border-style:inset;}"
                                        );


    qDebug() << tr("界面设定成功！");

    this->setWindowTitle("进样蠕动泵控制系统 ver5.0");
    this->setWindowIcon( QIcon("Instrument.ico") );  //设置软件图标

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);


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
void MainWindow::Read_Data()    //在“打开串口”按钮的槽函数里面进行 信号与Read_Data()的连接
{
    qDebug()<<"进入Read_Data()";
    QByteArray buf;
    buf = serial->readAll();

    QString string = buf;
    qDebug()<<"buf里的内容："+string;

    if(!buf.isEmpty())
    {
        QString str = ui->textEdit->toPlainText();
        str+=tr(buf);
        ui->textEdit->clear();
        ui->textEdit->append(str);
        qDebug()<<"buf 里面不为空";
        SaveToTxt( tr("接收") + ":" + tr(buf) );    //写到文件
    }
    else{
        qDebug()<<"buf 里面为空！";
    }
    buf.clear();
    qDebug()<<"完成Read_Data()一次";
}


void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);   //以“可读可写”方式打开串口
        //serial->open(QIODevice::ReadWrite);   //以“可读可写”方式打开串口

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
        ui->pushButton_searchDevice->setEnabled(false);
        ui->openButton->setText(tr("关闭串口"));   // *********改变 if语句 的判断条件************
        ui->sendButton->setEnabled(true);

        ui->lineEdit_commandOrder->setEnabled(true);
        ui->lineEdit_leftTime_PumpA->setEnabled(true);
        ui->lineEdit_leftTime_PumpB->setEnabled(true);
        ui->lineEdit_SamplingTime_PumpA->setEnabled(true);
        ui->lineEdit_samplingTime_PumpB->setEnabled(true);
        ui->lineEdit_setFrequency_PumpA->setEnabled(true);
        ui->lineEdit_setFrequency_PumpB->setEnabled(true);
        ui->checkBox_startDelay->setEnabled(true);
        ui->pushButton_PumpA->setEnabled(true);
        ui->pushButton_PumpB->setEnabled(true);

        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
        qDebug()<<"信号与槽连接完毕！";

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
        ui->pushButton_searchDevice->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));  // *********改变 if语句 的判断条件************
        ui->sendButton->setEnabled(false);

        ui->lineEdit_commandOrder->setEnabled(false);
        ui->lineEdit_leftTime_PumpA->setEnabled(false);
        ui->lineEdit_leftTime_PumpB->setEnabled(false);
        ui->lineEdit_SamplingTime_PumpA->setEnabled(false);
        ui->lineEdit_samplingTime_PumpB->setEnabled(false);
        ui->lineEdit_setFrequency_PumpA->setEnabled(false);
        ui->lineEdit_setFrequency_PumpB->setEnabled(false);
        ui->checkBox_startDelay->setEnabled(false);
        ui->pushButton_PumpA->setEnabled(false);
        ui->pushButton_PumpB->setEnabled(false);

        //我觉得从逻辑来讲，在关闭了串口之后，需要重新查找串口，查看一下是否有新的串口设备接入
        /*
        ui->PortBox->clear();    //step1.调用QComboBox的clear()方法来清除下拉选项
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //step2.查找可用的串口
        {
            QSerialPort serial;
            serial.setPort(info);
            if(serial.open(QIODevice::ReadWrite))
            {
                ui->PortBox->addItem(serial.portName());
                serial.close();
            }
        }*/

    }
}



void MainWindow::on_pushButton_PumpA_clicked()
{
    ui->textEdit_2->setText("a");
    on_sendButton_clicked();

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
void MainWindow::timerUpdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->label_timer->setText(timeStr);
}

void MainWindow::on_pushButton_searchDevice_clicked()  //搜索设备按钮
{
    ui->PortBox->clear();    //step1.调用QComboBox的clear()方法来清除下拉选项
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //step2.查找可用的串口
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());
            serial.close();
        }
    }
}

void MainWindow::on_pushButton_saveData_clicked()
{
    saveFileName = ui->lineEdit_savePath->text();
    if( saveFileName.length() == 0 )
    {
        saveFileName = "D://data.txt";
    }
    else if( saveFileName.length() > 4 )  //因为.txt就占4个字符
    {
        if( saveFileName.indexOf(".txt", ( saveFileName.length() - 4 )) == -1 ) // 输入的路径以.txt结尾
        {
            saveFileName = saveFileName + ".txt";
        }
    }
    else
    {
        saveFileName = saveFileName + ".txt";
    }

    bSaveFile = true;

    QDateTime localTime( QDateTime::currentDateTime() );
    SaveToTxt( localTime.toString("yy-MM-dd hh:mm:ss.zzz") + " Save:" );

    ui->pushButton_stopSave->setEnabled(true);
    ui->pushButton_saveData->setDisabled(true);
    ui->lineEdit_savePath->setDisabled(true);


}
void MainWindow::SaveToTxt(QString string)   //功能：将传递的字符串保存到文件中
{
    if( bSaveFile == false )  //首先判断是否需要保存
    {
        return;    //函数结束
    }

    QFile data(saveFileName);  //实例化出一个文件对象

    if( data.open( QFile::WriteOnly | QIODevice::Append ) )  //文件成功打开
    {
        QTextStream out( &data );
        out<<string<<"\r\n";
    }

    data.close();
}

void MainWindow::on_pushButton_stopSave_clicked()
{
    bSaveFile = false;
    ui->pushButton_saveData->setEnabled(true);
    ui->lineEdit_savePath->setEnabled(true);
    ui->pushButton_stopSave->setDisabled(true);
}
