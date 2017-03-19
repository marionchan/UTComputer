QT += core
QT -= gui

CONFIG += c++11

TARGET = UTComputer

CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pile.cpp \
    controleur.cpp \
    litterale.cpp \
    operateur.cpp \
    qtcomputer.cpp \
    qtoptions.cpp \
    qtvarwindow.cpp \
    qtprgwindow.cpp

HEADERS += \
    pile.h \
    controleur.h \
    litterale.h \
    operateur.h \
    qtcomputer.h \
    qtoptions.h \
    qtvarwindow.h \
    qtprgwindow.h

QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11

QT += widgets

