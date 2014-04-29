#-------------------------------------------------
#
# Project created by QtCreator 2014-03-03T10:08:04
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = DevTestQT
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
