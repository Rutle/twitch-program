#-------------------------------------------------
#
# Project created by QtCreator 2016-06-17T18:15:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = twitch-program
TEMPLATE = app
CONFIG += c++11
#CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    networkmanager.cpp \
    utilityprograms.cpp \
    settings.cpp \
    channel.cpp \
    stream.cpp

HEADERS  += mainwindow.hh \
    networkmanager.hh \
    utilityprograms.hh \
    settings.hh \
    channel.hh \
    stream.hh

FORMS    += mainwindow.ui
