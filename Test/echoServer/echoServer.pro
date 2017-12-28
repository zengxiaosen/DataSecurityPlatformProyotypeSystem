QT += core network
QT -= gui

CONFIG += c++11

TARGET = echoServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    echoserver.cpp

HEADERS += \
    echoserver.h
