#-------------------------------------------------
#
# Project created by QtCreator 2025-06-25T13:30:39
#
#-------------------------------------------------

QT       += core gui printsupport
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projet_POO
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    TableHachage.cpp \
    dialoginserer.cpp \
    dialogsupprimer.cpp \
    dialogrechercher.cpp \
    comparehash.cpp \
    qcustomplot.cpp \
    dialogafficher.cpp \
    pageaide.cpp

HEADERS  += mainwindow.h \
    TableHachage.h \
    dialoginserer.h \
    dialogsupprimer.h \
    dialogrechercher.h \
    comparehash.h \
    qcustomplot.h \
    dialogafficher.h \
    pageaide.h

FORMS    += mainwindow.ui \
    dialoginserer.ui \
    dialogsupprimer.ui \
    dialogrechercher.ui \
    comparehash.ui \
    dialogafficher.ui \
    pageaide.ui

CONFIG += c++11

RESOURCES += \
    image.qrc
