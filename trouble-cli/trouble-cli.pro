QT += core
QT -= gui

TARGET = trouble-cli
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

LIBS += -ldl

include(../common.pri)
include(../libtrouble.pri)
