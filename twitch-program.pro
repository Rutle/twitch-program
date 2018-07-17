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
    stream.cpp \
    widgets/channelinfo.cpp \
    widgets/marqueewidgetlabel.cpp \
    widgets/miniinfo.cpp \
    widgets/topgameslistdelegate.cpp \
    widgets/topgameslistmodel.cpp \
    program.cpp

HEADERS  += mainwindow.hh \
    networkmanager.hh \
    utilityprograms.hh \
    settings.hh \
    stream.hh \
    widgets/channelinfo.hh \
    widgets/marqueewidgetlabel.hh \
    widgets/miniinfo.hh \
    widgets/topgameslistdelegate.hh \
    widgets/topgameslistmodel.hh \
    program.hh \
    interface/programinterface.hh

FORMS    += mainwindow.ui

RESOURCES += \
    pictures.qrc

INCLUDEPATH += $$PWD/widgets \
               $$PWD/interface
