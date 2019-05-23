#-------------------------------------------------
#
# Project created by QtCreator 2019-04-02T13:11:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simplex
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bazisAndGetModel.cpp \
    loadTabFromFileBut.cpp \
    simplexstages.cpp \
    stepSimplex.cpp \
    LoadFileInt.cpp \
    simplexStagesINT.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
