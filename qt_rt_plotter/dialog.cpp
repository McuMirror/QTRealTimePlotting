//=============================================================================
// Filename:    dialog.cpp
// Created:	    8/20/2016
// Developer:   Cary Champlin
// Company:     Champlin Technologies LLC
// Copyright:   2016 Champlin Technologies LLC. All rights reserved.
//=============================================================================
// include files
//---------------------------------------------------------
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include "dialog.h"

#include "../qcustomplot.h"

#include <QDialog>
#include <QMainWindow>
#include <QtWidgets>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QTimer>
//#include <QDebug>

//---------------------------------------------------------
// Dialog class and constructor
//---------------------------------------------------------
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    dialog_ui(new Ui::Dialog)
{
    dialog_ui->setupUi(this);

    //-----------------------------------------------------
    // setup GUI dialog with initial status
    //-----------------------------------------------------
    dialog_ui->usbStatus->setText("Status: USB port isn't established.");
    dialog_ui->connectUSB->setText("Connect to USB port.");
    dialog_ui->startPressurePlot->setText("Start Plotting");
    dialog_ui->startPressurePlot->setDisabled(true);
    rtpUSB_is_available = false;
    rtpUSB_cancel_enable = false;
    connect(&pressure_time, SIGNAL(timeout()), this, SLOT(request_pressure_slot()));
}

//---------------------------------------------------------
// destructor
//---------------------------------------------------------
Dialog::~Dialog()
{
    delete dialog_ui;
}

//---------------------------------------------------------
// Slot: Find port with arduino usb device.
//---------------------------------------------------------
void Dialog::usbConnectionDataSlot()
{
    //-----------------------------------------------------
    // Searches com ports for usb arduino connection.
    // When port is found, its modes are configured.
    //-----------------------------------------------------
    if (rtpUSB_is_available == false)
    {
        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            if(serialPortInfo.hasVendorIdentifier())
            {
                if(serialPortInfo.vendorIdentifier() == arduino_due_vendor_id)
                {
                    rtpUSB_is_available = true;
                    rtpUSB_cancel_enable = false;
                    rtpUSB_port_name = serialPortInfo.portName();
                    rtpUSB->setPort(serialPortInfo);
                    rtpUSB->setBaudRate(QSerialPort::Baud115200);
                    rtpUSB->setDataBits(QSerialPort::Data8);
                    rtpUSB->setParity(QSerialPort::NoParity);
                    rtpUSB->setStopBits(QSerialPort::OneStop);
                    rtpUSB->setFlowControl(QSerialPort::HardwareControl);
                    rtpUSB->open(QIODevice::ReadWrite);
               }
            }
        }
    } else {
        dialog_ui->startPressurePlot->setEnabled(true);
        dialog_ui->usbStatus->setText("Status: USB port established.");
        dialog_ui->connectUSB->setText("Break USB connection.");
        dialog_ui->connectUSB->setEnabled(true);
        button_state = false;
        usbConnectionTimer.stop();
    }
}

//---------------------------------------------------------
// Slot: Make or Break button for USB connection.
//---------------------------------------------------------
void Dialog::on_connectUSB_released()
{
    //-----------------------------------------------------
    // Upon button press, set 1/2 second timer to check
    // for a usb connection.
    // While searching, a cancel button is displayed.
    // Once connected, button press will send 'Q' to  quit
    // and break USB connection.
    //-----------------------------------------------------
    if (rtpUSB_is_available == false){
        if (rtpUSB_cancel_enable == false){
            rtpUSB = new QSerialPort;

            dialog_ui->connectUSB->setText("Cancel search.");
            dialog_ui->usbStatus->setText("Status: Searching for USB port.");
            connect(&usbConnectionTimer, SIGNAL(timeout()), this, SLOT(usbConnectionDataSlot()));
            usbConnectionTimer.start(500);
            window_flag = false;
            rtpUSB_cancel_enable = true;
        } else {
            usbConnectionTimer.stop();
            disconnect(&usbConnectionTimer, SIGNAL(timeout()), this, SLOT(usbConnectionDataSlot()));
            dialog_ui->usbStatus->setText("Status: Search for USB port cancelled.");
            dialog_ui->connectUSB->setText("Connect to USB port.");
            rtpUSB_cancel_enable = false;
        }
    } else {
        rtpUSB_is_available = false;
        rtpUSB_cancel_enable = false;
        if(plotWindow2) {
            plotWindow2->close();
            plotWindow2 = NULL;
        }
        dialog_ui->usbStatus->setText("Status: USB port isn't established.");
        dialog_ui->connectUSB->setText("Connect to USB port.");
        dialog_ui->startPressurePlot->setText("Start Plotting");
        dialog_ui->startPressurePlot->setDisabled(true);
        rtpUSB->close();
        disconnect(&usbConnectionTimer, SIGNAL(timeout()), this, SLOT(usbConnectionDataSlot()));
    }
}

//---------------------------------------------------------
// Slot: Start or Pause button for plotting pressures.
// Pressure measurements occur every 10 msec.
//---------------------------------------------------------
void Dialog::on_startPressurePlot_released()
{
    if (window_flag == false) {
        plotWindow2 = new PlotWindow();
        plotWindow2->show();
        connect(this, &Dialog::get_pressure_data, plotWindow2, &PlotWindow::realtimeDataSlot);
        window_flag = true;
    }

    button_state = !button_state;
    if (button_state) {       // plotting started
        dialog_ui->connectUSB->setDisabled(true);
        pressureData = 0.0;
        first_read = true;
        pressure_time.start(20);
        plotWindow2->startPlotTimer();
        dialog_ui->startPressurePlot->setText("Pause Plotting");
        dialog_ui->usbStatus->setText("Status: Receiving pressure data for plotting.");
    } else {                  // plotting paused
        dialog_ui->connectUSB->setDisabled(false);
        pressure_time.stop();
        dialog_ui->startPressurePlot->setText("Re-Start Plotting");
        dialog_ui->usbStatus->setText("Status: Paused plotting pressure data.");
    }
}

//---------------------------------------------------------
// Slot: Request pressure measurements.
//---------------------------------------------------------
void Dialog::request_pressure_slot()
{
    //-----------------------------------------------------
    // Read pressure value from previous request allows
    // max time for pressure measurement to be received.
    // Skip read prior to first request.
    //-----------------------------------------------------
    if (first_read == false) {
        serialBuffer = rtpUSB->readAll();
        int chop_point = serialBuffer.indexOf("\n") - 1;
        serialBuffer1 = serialBuffer.left(chop_point);
        if (!serialBuffer1.isEmpty()) {
            pressureData = serialBuffer1.toDouble()/100;
            serialBuffer.clear();
            serialBuffer1.clear();
            emit get_pressure_data(pressureData);
        }
    } else {
        rtpUSB->clear();
        serialBuffer.clear();
        serialBuffer1.clear();
        first_read = false;
    }

    //-----------------------------------------------------
    // request a pressure measurement by sending cmd
    //-----------------------------------------------------
    rtpUSB->write("4\n");
}

void Dialog::closeEvent(QCloseEvent *bar)
{
    if(plotWindow2) {
        plotWindow2->close();
    }
    bar->accept();
}

