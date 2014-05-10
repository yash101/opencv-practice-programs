#-------------------------------------------------
#
# Project created by QtCreator 2014-05-08T11:01:43
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = DevScreen
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
LIBS += `pkg-config --libs opencv libfreenect`
LIBS += -lfreenect_sync
SOURCES += main.cpp

HEADERS += \
    FunctionsToolkit.hpp
