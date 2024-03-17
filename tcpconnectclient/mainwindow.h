#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

protected:
    void timerEvent(QTimerEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSound *messageSound;
    QSound *connectSound;

private slots:

    void on_sendMsg_clicked();

    void on_connect_clicked();

    void on_disconnect_clicked();


private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QLabel *m_status;
    QDateTime m_startTime; // Variable to store the start time
    qint64 m_onlineDuration; // Variable to store the online duration in milliseconds
    int m_timerId; // Timer ID for updating the online duration

       // Function to update the online duration
       void updateOnlineDuration();
};
#endif // MAINWINDOW_H
