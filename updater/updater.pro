#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T20:03:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = updater
TEMPLATE = app


SOURCES += main.cpp\
        mainw.cpp

HEADERS  += mainw.h

FORMS    += mainw.ui

TRANSLATIONS = CN.ts

RESOURCES += \
    resource.qrc
unix{
DEFINES += EXPORT_TO_BOARD
}
