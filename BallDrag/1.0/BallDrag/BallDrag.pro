#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T11:10:09
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = BallDrag
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    backend.cpp

LIBS += `pkg-config --libs opencv`

HEADERS += \
    backend.h
