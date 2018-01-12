#ifndef CLIENTLOGIN_H
#define CLIENTLOGIN_H

#include <QWidget>
#include "mqtt/qmqtt.h"
#include "mainwindow.h"

namespace Ui {
class ClientLogin;
}

class ClientLogin : public QWidget
{
    Q_OBJECT

public:
    explicit ClientLogin(QWidget *parent = 0);
    ~ClientLogin();
    //----------------------------------------
//    QMQTT::Client *mqtt_client = new QMQTT::Client();
    static ClientLogin *Instance();
    static ClientLogin *client_login;

private:
    Ui::ClientLogin *ui;
    //---------------------------------------
    void InitMainWindow();
    char *QstringToChar(QString);
    MainWindow *main_window = new MainWindow;

private slots:
    void LoginMQTTServer();
    void LogoutMQTTServer();
    void MQTTConnectSuccess();
    void MQTTConnectError(const QMQTT::ClientError &);
};

#endif // CLIENTLOGIN_H
