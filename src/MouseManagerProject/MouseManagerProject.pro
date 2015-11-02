#-------------------------------------------------
#
# Project created by QtCreator 2015-11-02T08:48:36
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MouseManagerProject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DeviceManager/DeviceManager.cpp \
    MouseListener/MouseListener.cpp \
    Singleton/Singleton.cpp \
    Facade/Facade.cpp

HEADERS  += mainwindow.h \
    DeviceManager/DeviceManager.h \
    Exception/Exception.h \
    Exception/OpenFileException.h \
    Exception/SendDeviceCommandException.h \
    MouseListener/MouseListener.h \
    Singleton/Singleton.h \
    Facade/Facade.h

FORMS    += mainwindow.ui
