#-------------------------------------------------
#
# Project created by QtCreator 2014-07-04T17:38:54
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ImageSender
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

LIBS += `pkg-config --libs opencv`
