#-------------------------------------------------
#
# Project created by QtCreator 2014-01-28T11:27:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TrackBalls
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp
