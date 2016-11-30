#-------------------------------------------------
#
# Project created by QtCreator 2016-11-30T14:56:14
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_workloadtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_workloadtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../../common.pri)
include(../../libtrouble.pri)
