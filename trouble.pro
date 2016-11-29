QT += core
QT -= gui

TARGET = trouble
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    tp.c \
    counter.cpp

QMAKE_CXXFLAGS += -std=c++0x \
 -finstrument-functions -finstrument-functions-exclude-file-list=qgenericatomic.h \

LIBS += -llttng-ust -ldl

HEADERS += \
    tp.h \
    counter.h
