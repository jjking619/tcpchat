#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_loginbtn_clicked()
{
    if(ui->username->text()=="admin" && ui->passwd->text()=="123456"){
            accept();
    }
}
