 #
 # Copyright 2016 Vyacheslav Verkhovin
 #
 # This file is part of MixCalc.
 #
 # MixCalc is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 #
 # RxCalc is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with RxCalc. If not, see <http://www.gnu.org/licenses/>.
 #

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
