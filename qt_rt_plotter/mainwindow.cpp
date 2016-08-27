//=============================================================================
// Filename:    mainwindow.cpp
// Created:	    8/20/2016
// Developer:   Cary Champlin
// Company:     Champlin Technologies LLC
// Copyright 2016, Champlin Technologies LLC. All rights reserved.
//=============================================================================
// include files
//-----------------------------------------------------------------------------
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include "dialog.h"

#include "../qcustomplot.h"

#include <QDialog>
#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QTimer>
//#include <QDebug>

//-----------------------------------------------------------------------------
// MainWindow class and constructor
//-----------------------------------------------------------------------------
PlotWindow::PlotWindow(QWidget *parent) :
  QMainWindow(parent),
plot_ui(new Ui::PlotWindow)
{
  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
  plot_ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  setupRealtimeDataDemo(plot_ui->customPlot);
  setWindowTitle("Real-Time Pressure Measurements");

  statusBar()->clearMessage();
  plot_ui->customPlot->replot();
}

void PlotWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  customPlot->graph(0)->setAntialiasedFill(false);

  customPlot->addGraph(); // blue dot
  customPlot->graph(1)->setPen(QPen(Qt::blue));
  customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);

  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(2);
  customPlot->axisRect()->setupFullAxesBox(true);
  customPlot->yAxis->setRange(-80,80);
}

void PlotWindow::startPlotTimer()
{
    plot_ui->customPlot->graph(0)->clearData();
    plot_ui->customPlot->graph(1)->clearData();
}

void PlotWindow::realtimeDataSlot(double value0)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    plot_ui->customPlot->graph(0)->addData(key, value0);
    plot_ui->customPlot->graph(1)->clearData();
    plot_ui->customPlot->graph(1)->addData(key, value0);
    plot_ui->customPlot->graph(0)->removeDataBefore(key-10);
    plot_ui->customPlot->xAxis->setRange(key+0.25, 10, Qt::AlignRight);
    plot_ui->customPlot->replot();
}

PlotWindow::~PlotWindow()
{
    delete plot_ui;
}

