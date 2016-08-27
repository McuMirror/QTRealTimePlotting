//=============================================================================
// Filename:    dialog.h
// Created:	    8/20/2016
// Developer:   Cary Champlin
// Company:     Champlin Technologies LLC
// Copyright:   2016 Champlin Technologies LLC. All rights reserved.
//=============================================================================
#ifndef DIALOG_H
#define DIALOG_H

//---------------------------------------------------------
// include files
//---------------------------------------------------------
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include "mainwindow.h"

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
QT_BEGIN_NAMESPACE

namespace Ui {
class Dialog;
}

QT_END_NAMESPACE

class PlotWindow;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void get_pressure_data(double pressureValue);

private slots:
    void usbConnectionDataSlot();
    void on_connectUSB_released();
    void on_startPressurePlot_released();
    void request_pressure_slot();

private:
    Ui::Dialog *dialog_ui;
    PlotWindow *plotWindow2 = NULL;
    void closeEvent(QCloseEvent *bar);

    QString rtpUSB_port_name;
    QString serialBuffer;
    QString serialBuffer1;

    QTimer pressure_time;
    QTimer usbConnectionTimer;

    QSerialPort *rtpUSB;

    bool rtpUSB_is_available;
    bool rtpUSB_cancel_enable;
    bool button_state;
    bool window_flag;
    bool first_read;

    static const quint16 arduino_due_vendor_id = 9025;
    double pressureData;

}; // class Dialog

#endif // DIALOG_H
