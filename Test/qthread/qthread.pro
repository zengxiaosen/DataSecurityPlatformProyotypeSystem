QT += core network widgets
QT -= gui

CONFIG += c++11

TARGET = qthread
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    widget.cpp

HEADERS += \
    widget.h
