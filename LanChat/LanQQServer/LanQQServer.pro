#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T15:22:33
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LCServer
TEMPLATE = app


SOURCES += main.cpp\
    tcpclientsocket.cpp \
    tcpserver.cpp \
    lcdbctrl.cpp \
    lcdb.cpp \
    lcserver.cpp

HEADERS  += \
    tcpclientsocket.h \
    tcpserver.h \
    DArrQueue.h \
    LCPdu.h \
    lcdbctrl.h \
    lcdb.h \
    lcserver.h \
    LCDBInfo.h

FORMS    += lcserver.ui
