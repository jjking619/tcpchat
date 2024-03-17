#include "tcplogin.h"
#include "ui_tcplogin.h"

tcplogin::tcplogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcplogin)
{
    ui->setupUi(this);
}

tcplogin::~tcplogin()
{
    delete ui;
}

void tcplogin::on_loginbtn_clicked()
{
    if(ui->username=="jkajka"&&ui->passwd=="123456"){
            accept();
    }
}
