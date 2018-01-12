#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientlogin.h"

#include <QList>
#include <QMessageBox>
#include <QNetworkInterface>

extern QMQTT::Client *mqtt_client;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitMainWindow();
    ui->radioButton_tcp->setChecked(true);
    ui->label_server->setText("服务器（IP：" + getHostIpAddress() + "，端口：8888）");
    tcpSocket = new QTcpSocket(this);
    tcpServer = new QTcpServer(this);
    udpSocket = new QUdpSocket(this);
    tcpServer->listen(QHostAddress::Any, 8888);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(TcpNewConnect()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(TcpDisConnect()));
    connect(ui->radioButton_tcp, SIGNAL(toggled(bool)), this, SLOT(ChangeToTcpServer()));
    connect(ui->radioButton_udp, SIGNAL(toggled(bool)), this, SLOT(ChangeToUdpServer()));
    connect(mqtt_client, SIGNAL(disconnected()), this, SLOT(MQTTDisconnect()));
    connect(mqtt_client, SIGNAL(received(const QMQTT::Message &)), this, SLOT(MQTTReadMessage(const QMQTT::Message &)));
}

QString MainWindow::getHostIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)  {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     if (strIpAddress.isEmpty())  {
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     }
     return strIpAddress;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitMainWindow()
{
    setFixedSize(534, 534);
    setWindowTitle("TCP/UDP->MQTT转发");
    setWindowIcon(QIcon(":/resource/log"));
}

void MainWindow::TcpNewConnect()
{
    tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(TcpReadMessage()));
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    QTableWidgetItem *item = new QTableWidgetItem(tcpSocket->peerAddress().toString().mid(7));
    ui->tableWidget->setItem(rowCount - 1, 0, item);
    ui->tableWidget->item(rowCount - 1, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont font("楷体", 12, 100);
    ui->tableWidget->item(rowCount - 1, 0)->setFont(font);
}

void MainWindow::TcpDisConnect() {
    ui->textBrowser->setText("连接断开");
}

void MainWindow::TcpReadMessage()
{
    QString recieve;
    recieve += tcpSocket->readAll();
    ui->label_from->setText("（TCP消息来自：" + tcpSocket->peerAddress().toString().mid(7) + "）");
    ui->textBrowser->setText(recieve);
    QMQTT::Message message(0, "ServerTopic", QString(recieve).toLatin1());
    if (mqtt_client->isConnectedToHost()) {
        mqtt_client->publish(message);
    }
}

void MainWindow::ChangeToTcpServer() {
    if (ui->radioButton_tcp->isChecked()) {
        disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(UdpReadMessage()));
        udpSocket->close();
        udpSocket = NULL;

        ui->label_from->setText("（TCP消息来自：0.0.0.0）");
        ui->textBrowser->clear();
        tcpSocket = new QTcpSocket(this);
        tcpServer = new QTcpServer(this);
        tcpServer->listen(QHostAddress::Any, 8888);
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(TcpNewConnect()));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(TcpDisConnect()));
    }
}

void MainWindow::ChangeToUdpServer() {
    if (ui->radioButton_udp->isChecked()) {
        disconnect(tcpServer, SIGNAL(newConnection()), this, SLOT(TcpNewConnect()));
        tcpServer->close();
        tcpSocket->close();
        tcpServer = NULL;
        tcpSocket = NULL;

        ui->label_from->setText("（UDP消息来自：0.0.0.0）");
        ui->textBrowser->clear();
        udpSocket = new QUdpSocket(this);
        bool bind = udpSocket->bind(QHostAddress(getHostIpAddress()), 8888);
        if (bind == false) {
            QMessageBox::about(this, "UDP", "UDP bind()失败!");
            return;
        }
        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(UdpReadMessage()));
    }
}

void MainWindow::UdpReadMessage() {
    ui->textBrowser->setText("read");
    QHostAddress address;
    char buf[1024];
    while(udpSocket->hasPendingDatagrams()) {
        memset(buf, 0, sizeof(buf));
        udpSocket->readDatagram(buf, sizeof(buf), &address);
        ui->label_from->setText("（UDP消息来自：" + address.toString() + "）");
        ui->textBrowser->setText(buf);
        QMQTT::Message message(0, "ServerTopic", QString(buf).toLatin1());
        if (mqtt_client->isConnectedToHost()) {
            mqtt_client->publish(message);
        }
    }
}

void MainWindow::MQTTReadMessage(const QMQTT::Message &message) {
    ui->label_from->setText("（消息来自：MQTT客户端）");
    ui->textBrowser->setText(message.payload());
}

void MainWindow::MQTTDisconnect() {
    QMessageBox::about(this, "连接失败", "MQTT掉线了!");
}
