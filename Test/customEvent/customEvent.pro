QT += core network widgets
QT -= gui

CONFIG += c++11

TARGET = customEvent
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    widget.cpp \
    customevent.cpp

HEADERS += \
    widget.h \
    customevent.h
