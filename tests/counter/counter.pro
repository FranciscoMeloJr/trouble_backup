#-------------------------------------------------
#
# Project created by QtCreator 2016-11-30T11:41:53
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_countertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_countertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../common.pri)
include(../../libtrouble.pri)
