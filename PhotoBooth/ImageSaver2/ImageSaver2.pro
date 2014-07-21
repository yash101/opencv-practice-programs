#-------------------------------------------------
#
# Project created by QtCreator 2014-06-28T17:34:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ImageSaver2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += `pkg-config --libs opencv`
LIBS += -lfreenect_sync

SOURCES += \
    main.cpp

HEADERS += \
    functionstoolkit.h

OTHER_FILES += \
    config.txt
