QT += core network
QT -= gui

CONFIG += c++11

TARGET = tcp
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp

HEADERS += \
    client.h
