QT += core
QT -= gui

TARGET = trouble
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp
QMAKE_CXXFLAGS += -std=c++0x
