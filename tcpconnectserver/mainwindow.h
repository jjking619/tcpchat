#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QtMultimedia/QSound>
#include <QIcon>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSound *messageSound;
    QSound *connectSound;

private slots:
    void on_setListen_clicked();
    void sendServerInfo();
    void on_sendMsg_clicked();
//    void updateServerInfo();

private:
    Ui::MainWindow *ui;
    QTcpServer *m_s; //服务端
    QTcpSocket *tcp_server; //套接字
    QList <QTcpSocket*> m_tcp;//多个客户端的套接字列表
    QLabel *m_status;
};
#endif // MAINWINDOW_H
