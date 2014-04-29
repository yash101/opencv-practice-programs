#-------------------------------------------------
#
# Project created by QtCreator 2014-04-28T16:03:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = untitled
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
