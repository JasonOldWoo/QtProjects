#-------------------------------------------------
#
# Project created by QtCreator 2013-10-04T23:54:27
#
#-------------------------------------------------
QT       += core gui sql webkit network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = connect_Mysql
TEMPLATE = app


SOURCES += main.cpp\
        mysql_widget.cpp \
    msgEcho.cpp

HEADERS  += mysql_widget.h \
    msgEcho.h \
    spcaframe.h

FORMS    += mysql_widget.ui

