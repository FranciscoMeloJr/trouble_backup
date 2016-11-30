QT += core
QT -= gui

TARGET = trouble
TEMPLATE = lib
CONFIG += sharedlib

SOURCES += \
    counter.cpp \
    workload.cpp \
    workloadpagefault.cpp

HEADERS += \
    counter.h \
    workload.h \
    workloadpagefault.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../common.pri)
