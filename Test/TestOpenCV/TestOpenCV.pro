#-------------------------------------------------
#
# Project created by QtCreator 2014-01-25T10:44:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TestOpenCV
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
