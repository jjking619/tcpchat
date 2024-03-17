#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //创建服务器对象
    setWindowTitle("服务器");
    ui->port->setText("8899");
    ui->ipBox->setStyleSheet("color:blue");
    ui->ipBox->setStyleSheet("background-color: rgb(6,163,220)");
    ui->setListen->setStyleSheet("background-color: rgb(6,163,220)");
    ui->sendMsg->setStyleSheet("background-color: rgb(6,163,220)");
    ui->port->setStyleSheet("color:blue");
    ui->msg->setStyleSheet("border:red");
    ui->ip->setStyleSheet("color:blue");
    ui->port->setStyleSheet("color:blue");
    ui->listWidget->setStyleSheet("border:2px solid blue");     //改变边框颜色

    QList <QHostAddress> addressList = QNetworkInterface::allAddresses();     //接收本机所有Ip地址
    QStringList addressList_str;
    for(int i = 0; i < addressList.size(); i++)
    {
        if(addressList.at(i).isNull()) continue;   //地址为空则跳过
        if(addressList.at(i).protocol() != QAbstractSocket::IPv4Protocol) continue;   //协议族不是ipv4的则跳过
        addressList_str.append(addressList.at(i).toString());      //符合条件的地址添加到列表中
    }
    ui->ipBox->addItems(addressList_str);   //将地址列表添加到地址列表框中
    messageSound = new QSound(":/sounds/iphone.wav", this);
    connectSound = new QSound(":/sounds/keke.wav", this);
    this->setWindowIcon(QIcon(":/image/qq.png"));
    //新的客户端连接
    m_s=new QTcpServer(this);
    connect(m_s,QTcpServer::newConnection,this,[=](){
        tcp_server=m_s->nextPendingConnection(); //获取当前连接的客户端套接字
        m_tcp.push_back(tcp_server); //将连接的客户端放入客户端列表中
        QString str = tcp_server->peerAddress().toString() + ": " + QString::number(tcp_server->peerPort()) + "已经连接";
        ui->listWidget->addItem(str);  //将连接信息放在列表窗口中
        m_status->setPixmap(QPixmap(":/连接状态-已连接.png").scaled(20,20));
    // Update server information
//         updateServerInfo();
    //Send server information to the client
         sendServerInfo();


    connect(tcp_server,QTcpSocket::readyRead,this,[=]() // //检测是否可以接收数据
    {
        QTcpSocket* currentClint;
        QByteArray arr;
        QString str;
        if(!m_tcp.isEmpty())      //有客户端存在
        {
            for(int i = 0; i < m_tcp.count(); i++)     //服务端接收信息
            {
                arr = m_tcp.at(i)->readAll();      //接收客户端发送的字节信息
                if(arr.isNull())  continue;   //空代表不是该客户端发送
//                messSound->play();
                currentClint = m_tcp.at(i);
                str = QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss") + '\n' + arr.data();
                break;
            }
            ui->record->append(str);     //显示信息
            for(int i = 0; i < m_tcp.count(); i++)     //给其它客户端发送信息
            {
                QTcpSocket *temp = m_tcp.at(i);
                if(currentClint == temp)  continue;      //遇到自己就跳过
                temp->write(str.toUtf8());   //发送信息
            }
        }
//              QByteArray data=tcp_server->readAll();
//              ui->record->append("客户端说"+data);
       });

    connect(tcp_server,QTcpSocket::disconnected,this,[=](){
                ui->setListen->setDisabled(false);
                QString closestr=QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss") + ' ' + "client close";
                 tcp_server->close();
                 tcp_server->deleteLater();  //delete m_tcp
                ui->listWidget->addItem(closestr);
                m_status->setPixmap(QPixmap(":/连接状态-未连接.png").scaled(20,20));
                connectSound->play();
       });

        });
    //状态栏
    m_status =new QLabel;
    m_status->setPixmap(QPixmap(":/连接状态-未连接.png").scaled(20,20));
    ui->statusBar->addWidget(new QLabel("连接状态:"));
    ui->statusBar->addWidget(m_status);
}

void MainWindow::sendServerInfo() //给客户端发送用户名信息等
{
    // Create and populate the server info data structure
    QJsonObject serverInfo;
    serverInfo["username"] = "admin";
    serverInfo["loginTime"] = QDateTime::currentDateTime().toString("dddd yyyy.MM.dd HH:mm:ss");
    serverInfo["onlineDuration"] = "1 hour"; //You can calculate the online duration

    QJsonDocument doc(serverInfo);
    QByteArray jsonData = doc.toJson();

    // Send the serialized data to the client
    tcp_server->write(jsonData);
    messageSound->play();
}

//void MainWindow::updateServerInfo() {
//    // Set server information
//    QString username = "admin";
//    QString loginTime = QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss");
//    QString onlineDuration = "1 hour"; // You can calculate the online duration
//    ui->username->setText(username);
//    ui->loginTime->setText(loginTime);
//    ui->onlineDuration->setText(onlineDuration);
//}


void MainWindow::on_setListen_clicked()
{
       unsigned short port = ui->port->text().toUShort();
       m_s->listen(QHostAddress::Any,port);
       ui->setListen->setDisabled(true);
}

void MainWindow::on_sendMsg_clicked()
{
       QString msg=ui->msg->toPlainText();

       // Get username and login time
       QString username ="admin"; // Assuming username is displayed in a QLabel
       QString currentTime = QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss"); // Assuming login time is displayed in a QLabel
         // Construct message with username and login time
       QString completeMsg = QString("%1<%2>\n%3").arg(username, currentTime, msg);
         // Send the complete message
       tcp_server->write(completeMsg.toUtf8());
       ui->record->append("服务器端"+completeMsg);
       messageSound->play();
}
MainWindow::~MainWindow()
{
    delete ui;
}
