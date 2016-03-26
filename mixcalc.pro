#-------------------------------------------------
#
# Project created by QtCreator 2016-03-18T19:48:44
#
#-------------------------------------------------

TARGET = mixcalc
VERSION  = 0.1.0
TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"MixCalc\\\"

SOURCES +=  sources/main.cpp\
            sources/mainwindow.cpp \
            sources/mygraphicview.cpp \
            sources/myrect.cpp

HEADERS +=  sources/mainwindow.h \
            sources/mygraphicview.h \
            sources/myrect.h

FORMS += resources/mainwindow.ui

RESOURCES += resources/icons.qrc

OTHER_FILES +=  README.md \
                LICENSE

QMAKE_CFLAGS += -std=c++11
