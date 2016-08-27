//=============================================================================
// Filename:    main.cpp
// Created:	    8/20/2016
// Developer:   Cary Champlin
// Company:     Champlin Technologies LLC
// Copyright:   2016 Champlin Technologies LLC. All rights reserved.
//=============================================================================
#include <QApplication>
#include "dialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Dialog w;
    w.setFixedSize(400,80);
    w.setWindowTitle("Real-Time USB Pressure Monitor");
    w.setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    w.show();

    return app.exec();
}
