#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("进样蠕动泵控制系统 登录界面");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginButton_clicked()
{
    if( ui->usrLineEdit->text() == tr("浙江大学") && ui->pwdLineEdit->text() == tr("1234") )
    {
        accept();
    }
    else{
        QMessageBox::warning(this, tr("警告！"), tr("用户名或密码错误！"), QMessageBox::Yes );
    }
}
