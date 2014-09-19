#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T18:09:45
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = pdf_annotate_extract
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += /usr/include/poppler/qt5
LIBS        += -L/usr/lib -lpoppler-qt5
