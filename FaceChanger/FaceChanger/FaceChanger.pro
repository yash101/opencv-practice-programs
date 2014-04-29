#-------------------------------------------------
#
# Project created by QtCreator 2014-04-02T13:19:27
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FaceChanger
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
