#include "clientlogin.h"
#include <QApplication>

QMQTT::Client *mqtt_client;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    mqtt_client = new QMQTT::Client();
    ClientLogin *w = ClientLogin::Instance();
    w->show();

    return a.exec();
}
