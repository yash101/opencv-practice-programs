#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T19:09:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = LearnThreading
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

libs += `pkg-config --libs opencv`

SOURCES += main.cpp
