#-------------------------------------------------
#
# Project created by QtCreator 2013-08-31T08:20:26
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snakePart
TEMPLATE = app


SOURCES += main.cpp\
        snake.cpp \
    mydb.cpp

HEADERS  += snake.h \
    mydb.h

FORMS    += snake.ui
