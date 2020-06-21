#-------------------------------------------------
#
# Project created by QtCreator 2016-01-27T13:35:50
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPort
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h

FORMS    += mainwindow.ui \
    logindialog.ui

RESOURCES += \
    image/image.qrc \
    icon.qrc
RC_FILE=icon.rc
