#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T14:53:54
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = VirtualWhiteboard
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
