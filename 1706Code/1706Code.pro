#-------------------------------------------------
#
# Project created by QtCreator 2014-02-06T14:49:51
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 1706Code
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
LIBS += `pkg-config --libs opencv`
