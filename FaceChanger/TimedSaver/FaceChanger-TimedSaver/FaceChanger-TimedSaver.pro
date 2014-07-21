#-------------------------------------------------
#
# Project created by QtCreator 2014-06-25T22:09:49
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FaceChanger-TimedSaver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    functionstoolkit.cpp

HEADERS += \
    functionstoolkit.h

LIBS += `pkg-config --libs opencv`
LIBS += -lfreenect_sync
