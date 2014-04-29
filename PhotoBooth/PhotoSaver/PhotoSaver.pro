#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T11:12:01
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = PhotoSaver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
