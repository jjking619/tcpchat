#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("客户端");
    ui->port->setText("8899");
    ui->ip->setText("127.0.0.1");
    ui->disconnect->setDisabled(true);
    ui->port->setText("8899");
    ui->ip->setStyleSheet("background-color: rgb(6,163,220)");
    ui->sendMsg->setStyleSheet("background-color: rgb(6,163,220)");
    ui->connect->setStyleSheet("background-color: rgb(6,163,220)");
    ui->disconnect->setStyleSheet("background-color: rgb(6,163,220)");
    ui->port->setStyleSheet("color:blue");
    ui->username->setStyleSheet("color:blue");
    ui->loginTime->setStyleSheet("color:blue");
    ui->onlineDuration->setStyleSheet("color:blue");
    ui->msg->setStyleSheet("border:red");
    ui->ip->setStyleSheet("color:blue");
    ui->listWidget->setStyleSheet("border:2px solid blue");     //改变边框颜色
    // Initialize online duration variables
    m_onlineDuration = 0;
    m_timerId = startTimer(1000); // Start a timer to update the online duration every second
    messageSound = new QSound(":/sounds/iphone.wav", this);
    connectSound = new QSound(":/sounds/keke.wav", this);

    this->setWindowIcon(QIcon(":/image/qq.png"));


    //创建服务器对象
    socket=new QTcpSocket(this);
    connect(socket,QTcpSocket::disconnected,this,[=](){
              socket->close();
              socket->deleteLater();  //delete m_tcp
              m_status->setPixmap(QPixmap(":/连接状态-未连接.png").scaled(20,20));
              ui->record->append("已经与服务器断开连接.....");
              connectSound->play();
              ui->connect->setDisabled(false);
              ui->disconnect->setEnabled(false);


    });
    connect(socket,QTcpSocket::connected,this,[=](){
              m_status->setPixmap(QPixmap(":/连接状态-已连接.png").scaled(20,20));
              ui->record->append("已经连接成功到服务器.....");
              connectSound->play();
              ui->connect->setDisabled(true);
              ui->disconnect->setEnabled(true);
    });


    //状态栏
    m_status =new QLabel;
    m_status->setPixmap(QPixmap(":/连接状态-未连接.png").scaled(20,20));
    ui->statusBar->addWidget(new QLabel("连接状态:"));
    ui->statusBar->addWidget(m_status);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId) {
        // Update the online duration
        updateOnlineDuration();
    }
}

void MainWindow::updateOnlineDuration()
{
    // Calculate the current online duration in seconds
        qint64 durationSeconds = m_startTime.secsTo(QDateTime::currentDateTime());

        // Convert seconds to hours, minutes, and remaining seconds
        qint64 hours = durationSeconds / 3600;
        qint64 minutes = (durationSeconds % 3600) / 60;
        qint64 seconds = durationSeconds % 60;

        // Update the UI to display the online duration in hours, minutes, and seconds
        QString durationString = QString("%1:%2:%3")
                                     .arg(hours, 2, 10, QLatin1Char('0'))
                                     .arg(minutes, 2, 10, QLatin1Char('0'))
                                     .arg(seconds, 2, 10, QLatin1Char('0'));
        ui->onlineDuration->setText(durationString);
}

void MainWindow::on_sendMsg_clicked()
{
        QString msg=ui->msg->toPlainText();

        // Get username and login time
        QString username = ui->username->text(); // Assuming username is displayed in a QLabel
        QString currentTime = QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss"); // Assuming login time is displayed in a QLabel

          // Construct message with username and login time
        QString completeMsg = QString("%1<%2>\n%3").arg(username, currentTime, msg);

          // Send the complete message
        socket->write(completeMsg.toUtf8());
        ui->record->append("客户端:" + completeMsg);
        messageSound->play();
}

void MainWindow::on_connect_clicked()
{
    m_startTime = QDateTime::currentDateTime(); // Record the start time
    QString ip= ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    // 创建新的套接字对象以支持重新连接
     if (socket || socket->state() == QAbstractSocket::UnconnectedState) {
         socket= new QTcpSocket(this);
         //读取服务端发送的信息
         connect(socket, &QTcpSocket::readyRead, this, [=]() {
                QByteArray data = socket->readAll();
                // Parse JSON data
                ui->record->append("服务器说"+(QString)data);
                QJsonParseError error;
                QJsonDocument doc = QJsonDocument::fromJson(data, &error);
                if (error.error == QJsonParseError::NoError && doc.isObject()) {
                    QJsonObject obj = doc.object();
                    // Extract server information
                    QString username = obj["username"].toString();
                    QString loginTime = obj["loginTime"].toString();
                    QString onlineDuration = obj["onlineDuration"].toString();
                    // Display server information in UI
                    ui->username->setText(username);
                    ui->loginTime->setText(loginTime);
                    ui->onlineDuration->setText(onlineDuration);
                     messageSound->play();
                } else {
                    qDebug() << "Error parsing server information JSON:" << error.errorString();
                }
            });
         // 重新关联已连接和断开连接的信号槽
         connect(socket, &QTcpSocket::connected, this, [=](){
             m_status->setPixmap(QPixmap(":/连接状态-已连接.png").scaled(20,20));
             ui->record->append("已经连接成功到服务器.....");
             ui->connect->setEnabled(false);
             ui->disconnect->setEnabled(true);
             connectSound->play();
         });
         connect(socket, &QTcpSocket::disconnected, this, [=](){
             socket->close();
             socket->deleteLater();  // delete m_tcp
             m_status->setPixmap(QPixmap(":/连接状态-未连接.png").scaled(20,20));
             ui->record->append("已经与服务器断开连接.....");
             ui->connect->setEnabled(true);
             ui->disconnect->setEnabled(false);
             // 解除已连接和断开连接的信号槽关联
             disconnect(socket, &QTcpSocket::connected, this, nullptr);
             disconnect(socket, &QTcpSocket::disconnected, this, nullptr);
             connectSound->play();
         });
     }
    socket->connectToHost(QHostAddress(ip),port);
}

void MainWindow::on_disconnect_clicked()
{
   // Stop tracking online duration when disconnected
    killTimer(m_timerId);
    ui->listWidget->addItem("clint disconnected");
    socket->close();
    socket->deleteLater();
    ui->connect->setDisabled(false);
    ui->disconnect->setEnabled(false);
    connectSound->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

