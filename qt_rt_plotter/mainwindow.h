//=====================================================================
// Filename:    mainwindow.h
// Created:	    8/20/2016
// Developer:   Cary Champlin
// Company:     Champlin Technologies LLC
// Copyright 2016, Champlin Technologies LLC. All rights reserved.
//=====================================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//---------------------------------------------------------
// include files
//---------------------------------------------------------
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include "dialog.h"

#include "../qcustomplot.h"

#include <QDialog>
#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QTimer>
//#include <QDebug>

//---------------------------------------------------------
// namespace and class
//---------------------------------------------------------
namespace Ui {
class PlotWindow;
}

class Dialog;

class PlotWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit PlotWindow(QWidget *parent = 0);
  ~PlotWindow();
  void setupRealtimeDataDemo(QCustomPlot *customPlot);
  void startPlotTimer();

public slots:
  void realtimeDataSlot(double value0);

private:
  Ui::PlotWindow *plot_ui;
  double value0;

}; // class PlotWindow

#endif // MAINWINDOW_H
