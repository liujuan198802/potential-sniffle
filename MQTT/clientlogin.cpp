#include "synchapi.h"
#include "clientlogin.h"
#include "ui_clientlogin.h"

#include <QTime>
#include <QTimer>
#include <QMessageBox>

extern QMQTT::Client *mqtt_client;

ClientLogin::ClientLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientLogin)
{
    ui->setupUi(this);
    InitMainWindow();
    connect(mqtt_client, SIGNAL(connected()), this, SLOT(MQTTConnectSuccess()));
    connect(mqtt_client, SIGNAL(error(QMQTT::ClientError)), this, SLOT(MQTTConnectError(const QMQTT::ClientError &)));
    connect(ui->pushButton_login, SIGNAL(clicked(bool)), this, SLOT(LoginMQTTServer()));
    connect(ui->pushButton_logout, SIGNAL(clicked(bool)), this, SLOT(LogoutMQTTServer()));
}

ClientLogin::~ClientLogin()
{
    delete ui;
}

ClientLogin *ClientLogin::client_login = NULL;

ClientLogin *ClientLogin::Instance()
{
    if (client_login == NULL) {
        client_login = new ClientLogin;
    }
    return client_login;
}

void ClientLogin::InitMainWindow()
{
    setFixedSize(534, 493);
    setWindowTitle("TCP/UDP->MQTT转发");
    setWindowIcon(QIcon(":/resource/log"));

    char *server = (char *)malloc(20);
    memset(server, 0, 20);
    FILE *fp_server = fopen("server.txt", "a+");
    fread(server, 1, 20, fp_server);
    QString str_server = server;
    ui->lineEdit_server->setText(str_server);
    fclose(fp_server);
    free(server);

    char *port = (char *)malloc(20);
    memset(port, 0, 20);
    FILE *fp_port = fopen("port.txt", "a+");
    fread(port, 1, 20, fp_port);
    QString str_port = port;
    ui->lineEdit_port->setText(str_port);
    fclose(fp_port);
    free(port);

    char *userName = (char *)malloc(20);
    memset(userName, 0, 20);
    FILE *fp_userName = fopen("userName.txt", "a+");
    fread(userName, 1, 20, fp_userName);
    QString str_userName = userName;
    ui->lineEdit_userName->setText(str_userName);
    fclose(fp_userName);
    free(userName);

    char *password = (char *)malloc(20);
    memset(password, 0, 20);
    FILE *fp_password = fopen("password.txt", "a+");
    fread(password, 1, 20, fp_password);
    QString str_password = password;
    ui->lineEdit_password->setText(str_password);
    fclose(fp_password);
    free(password);
}

//Qstring类型转char *类型
char *ClientLogin::QstringToChar(QString str)
{
    QByteArray byteArray = str.toLatin1();
    return byteArray.data();
}

void ClientLogin::LoginMQTTServer()
{

    if (ui->lineEdit_server->text().isEmpty() || ui->lineEdit_port->text().isEmpty()
            || ui->lineEdit_userName->text().isEmpty() || ui->lineEdit_password->text().isEmpty()) {
        QMessageBox::about(this, "登录失败", "服务器、端口、用户名、密码不能为空!");
        return;
    }
    char *server = QstringToChar(ui->lineEdit_server->text());
    FILE *fp_server = fopen("server.txt", "w+");
    fwrite(server, strlen(server) + 1, 1, fp_server);
    fclose(fp_server);

    char *port = QstringToChar(ui->lineEdit_port->text());
    FILE *fp_port = fopen("port.txt", "w+");
    fwrite(port, strlen(port) + 1, 1, fp_port);
    fclose(fp_port);

    char *userName = QstringToChar(ui->lineEdit_userName->text());
    FILE *fp_useraName = fopen("userName.txt", "w+");
    fwrite(userName, strlen(userName) + 1, 1, fp_useraName);
    fclose(fp_useraName);

    char *password = QstringToChar(ui->lineEdit_password->text());
    FILE *fp_password = fopen("password.txt", "w+");
    fwrite(password, strlen(password) + 1, 1, fp_password);
    fclose(fp_password);

    QHostAddress host(ui->lineEdit_server->text());
    QString str = ui->lineEdit_port->text();
    bool ok = true;
    quint16 port1 = str.toInt(&ok, 10);
    mqtt_client->setHost(host);
    mqtt_client->setPort(port1);
    mqtt_client->setUsername(ui->lineEdit_userName->text());
    mqtt_client->setPassword(password);
    mqtt_client->keepAlive();
    mqtt_client->setAutoReconnect(true);
    mqtt_client->connectToHost();
}

void ClientLogin::MQTTConnectSuccess() {
    mqtt_client->subscribe("ClientTopic", 2);
    main_window->show();
    close();
}

void ClientLogin::MQTTConnectError(const QMQTT::ClientError &error) {
    QString ErrorCode[] = {
        "UnknownError",
        "SocketConnectionRefusedError",
        "SocketRemoteHostClosedError",
        "SocketHostNotFoundError",
        "SocketAccessError",
        "SocketResourceError",
        "SocketTimeoutError",
        "SocketDatagramTooLargeError",
        "SocketNetworkError",
        "SocketAddressInUseError",
        "SocketAddressNotAvailableError",
        "SocketUnsupportedSocketOperationError",
        "SocketUnfinishedSocketOperationError",
        "SocketProxyAuthenticationRequiredError",
        "SocketSslHandshakeFailedError",
        "SocketProxyConnectionRefusedError",
        "SocketProxyConnectionClosedError",
        "SocketProxyConnectionTimeoutError",
        "SocketProxyNotFoundError",
        "SocketProxyProtocolError",
        "SocketOperationError",
        "SocketSslInternalError",
        "SocketSslInvalidUserDataError",
        "SocketTemporaryError"
    };
    ui->label_error->setText("*登录失败：" + ErrorCode[error]);
}

void ClientLogin::LogoutMQTTServer()
{
     close();
}
