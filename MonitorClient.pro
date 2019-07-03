#-------------------------------------------------
#
# Project created by QtCreator 2019-07-01T11:52:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonitorClient
TEMPLATE = app


SOURCES += main.cpp \
    Monitor.cpp \
    mainwindow.cpp

HEADERS  += \
    Monitor.h \
    mainwindow.h

FORMS    += \
    Monitor.ui \
    mainwindow.ui
