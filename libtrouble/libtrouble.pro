QT += core
QT -= gui

TARGET = trouble
TEMPLATE = lib
CONFIG += sharedlib

SOURCES += \
    counter.cpp

HEADERS += \
    counter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../common.pri)
