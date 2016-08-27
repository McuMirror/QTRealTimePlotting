#=============================================================================
# Filename:     qt-rt-plotter.pro
# Created:      8/20/2016
# Developer:    Cary Champlin
# Company:      Champlin Technologies LLC
# Copyright 2016, Champlin Technologies LLC. All rights reserved.
#=============================================================================

QT       += core gui serialport widgets printsupport

CONFIG += embed_manifest_exe

TARGET = qt-rt-plotter
TEMPLATE = app

SOURCES += main.cpp\
        dialog.cpp \
    mainwindow.cpp \
    ../qcustomplot.cpp

HEADERS  += dialog.h \
    mainwindow.h \
    ../qcustomplot.h

FORMS    += dialog.ui \
    mainwindow.ui
