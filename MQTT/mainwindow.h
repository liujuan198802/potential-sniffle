#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mqtt/qmqtt.h"
#include<QtNetwork/QTcpSocket>
#include<QtNetwork/QTcpServer>
#include<QtNetwork/QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //---------------------------------------
    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;
    QUdpSocket *udpSocket;

private:
    Ui::MainWindow *ui;
    //---------------------------------------
    void InitMainWindow();
    QString getHostIpAddress();

private slots:
    void TcpNewConnect();
    void TcpDisConnect();
    void TcpReadMessage();
    void UdpReadMessage();
    void ChangeToTcpServer();
    void ChangeToUdpServer();
    void MQTTDisconnect();
    void MQTTReadMessage(const QMQTT::Message &);
};

#endif // MAINWINDOW_H
