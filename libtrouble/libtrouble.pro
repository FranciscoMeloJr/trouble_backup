QT += core
QT -= gui

TARGET = trouble
TEMPLATE = lib
CONFIG += sharedlib

SOURCES += \
    counter.cpp \
    workload.cpp \
    workloadpagefault.cpp \
    workloadcachemisses.cpp \
    workloadinstructions.cpp \
    createfile.cpp

HEADERS += \
    counter.h \
    workload.h \
    workloadpagefault.h \
    workloadcachemisses.h \
    workloadinstructions.h \
    createfile.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../common.pri)
