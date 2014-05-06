#-------------------------------------------------
#
# Project created by QtCreator 2014-05-05T12:29:53
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FaceChanger
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv libfreenect libusb`
LIBS += -lfreenect_sync


SOURCES += main.cpp \
    functionstoolkit.cpp

HEADERS += \
    functionstoolkit.h
