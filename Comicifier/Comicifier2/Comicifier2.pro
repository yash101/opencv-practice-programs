#-------------------------------------------------
#
# Project created by QtCreator 2014-05-01T09:44:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Comicifier2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
