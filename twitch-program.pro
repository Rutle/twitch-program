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
    mainwindowminimal.cc \
    networkmanager.cpp \
    program.cc \
    selectorwindow.cc \
    utilityprograms.cpp \
    settings.cpp \
    stream.cpp \
    widgets/channelinfo.cpp \
    widgets/clickablelabel.cc \
    widgets/infomessage.cc \
    widgets/marqueewidgetlabel.cpp \
    widgets/miniinfo.cpp \
    widgets/topgameslistdelegate.cpp \
    widgets/topgameslistmodel.cpp \
    programmodel.cpp \
    widgetbuilder.cc

HEADERS  += mainwindow.hh \
    interface/programinterface.hh \
    interface/programmodelinterface.hh \
    mainwindowminimal.hh \
    networkmanager.hh \
    program.hh \
    selectorwindow.hh \
    utilityprograms.hh \
    settings.hh \
    stream.hh \
    widgets/channelinfo.hh \
    widgets/clickablelabel.hh \
    widgets/infomessage.hh \
    widgets/marqueewidgetlabel.hh \
    widgets/miniinfo.hh \
    widgets/topgameslistdelegate.hh \
    widgets/topgameslistmodel.hh \
    programmodel.hh \
    widgetbuilder.hh

FORMS    += mainwindow.ui \
    mainwindowminimal.ui \
    selectorwindow.ui

RESOURCES += \
    pictures.qrc

INCLUDEPATH += $$PWD/widgets \
               $$PWD/interface
