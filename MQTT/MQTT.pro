#-------------------------------------------------
#
# Project created by QtCreator 2018-01-10T08:47:14
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MQTT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientlogin.cpp

HEADERS  += mainwindow.h \
    mqtt/qmqtt_client.h \
    mqtt/qmqtt_global.h \
    mqtt/qmqtt_message.h \
    clientlogin.h

FORMS    += mainwindow.ui \
    clientlogin.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lqmqtt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lqmqttd
else:unix: LIBS += -L$$PWD/lib/ -lqmqtt

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lqmqtt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lqmqttd
else:unix: LIBS += -L$$PWD/lib/ -lqmqtt

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

RESOURCES += \
    resourse.qrc
