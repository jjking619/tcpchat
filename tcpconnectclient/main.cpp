#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;//主界面
    login tcplogin; //登录界面
//    w.show();
    if (tcplogin.exec() == QDialog::Accepted) {	// 如果符合登录条件，login执行后，必返回QDialog::Accepted
            w.show();	// 主界面显示
            return a.exec();
        }
    else return 0;
}
