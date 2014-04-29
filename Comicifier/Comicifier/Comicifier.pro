#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T16:27:53
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Comicifier
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
