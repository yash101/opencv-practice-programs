#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T07:13:06
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = MrPranay
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

LIBS += `pkg-config --libs opencv`
LIBS += -lfreenect_sync

HEADERS += \
    functionstoolkit.h
