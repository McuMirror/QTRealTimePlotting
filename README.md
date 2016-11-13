# Plotting Pressure Measurements in Real-Time via USB
The 'Real-Time USB Pressure Monitor' software program is used to establish a USB connection with an external pressure transducer and plot pressure measurements on a plot window that updates in real-time as a strip chart. The main GUI is a simple dialog that guides the user through the top-level steps to establish a USB connection and start/pause the plotting. A second window containing a strip chart is created when the 'Start Plotting' button is clicked. The strip chart window is resizable and plotting can be paused for review and restarted. Although pressure units and ranges can easily be modified in software, the current example application records pressure between -75 to 75 cm-H2O as indicated on the vertical axis of the strip chart window. Time is labeled every two seconds on the horizontal axis. The GUI ‘look-n-feel’ matches the current windows desktop theme (apparently, I like the really old desktop themes from Windows).

![Dialog_01](https://github.com/CaryChamplin/QTRealTimePlotting/blob/develop/dialog_01.png)

![Plot_01](https://github.com/CaryChamplin/QTRealTimePlotting/blob/develop/plot_01.png)

###Operation
It was definitely my intent to keep the GUI dialog simple and its operation intuitive. The initial GUI provides a button to establish a USB port connection to the pressure transducer.

![Dialog_01](https://github.com/CaryChamplin/QTRealTimePlotting/blob/develop/dialog_01.png)

Clicking on the ‘Connect to USB port’ button causes the software to search for a USB port connection to the pressure transducer every 0.5 second until a USB connection is established or the ‘Cancel search’ button is clicked.

![Dialog_02](https://github.com/CaryChamplin/QTRealTimePlotting/blob/develop/dialog_02.png)

After a USB connection is established, the ‘Start Plotting’ button is enabled. Clicking on the ‘Start Plotting’ button brings up the strip chart plotting window and starts plotting pressure measurements. In the current example program, a simple handshake is used:-	Every 20 msec (i.e., 50 samples/second), transmit request (“4\n”) via USB port for a pressure measurement.-	Receive character stream between 1 to 5 characters consisting of a possible negative sign and up to four digits (-7500 to 7500). Dividing the received integer by 100 yields pressure in units of cm-H2O.

![Dialog_03](https://github.com/CaryChamplin/QTRealTimePlotting/blob/develop/dialog_03.png)

After a plot is started, the ‘Pause Plotting’ button is enabled. Plotting can be paused and restarted. The plotting window can be resized in real-time.

![Dialog_04](https://github.com/CaryChamplin/QTRealTimePlotting/blob/develop/dialog_04.png)

If the ‘Pause Plotting’ button is clicked, two options become enabled: 'Break USB Connection' and 'Re-Start Plotting'. Clicking on the ‘Break USB connection’ button will close the plotting window.

![Dialog_05](https://github.com/CaryChamplin/QTRealTimePlotting/blob/develop/dialog_05.png)

###Software Development Framework
[Qt](https://www.qt.io) is a cross-platform development environment that can be used for developing application software with GUIs. Software is primarily written in C++ (such as the ‘Real-Time USB Pressure Monitor’ application that runs on Windows). The only unique concept in using Qt is the use of signals and slots to handle the events. Numerous examples are provided by [Qt](https://www.qt.io).
-	[Qt Creator](https://www.qt.io/ide/)
-	[Qt Signals and Slots](http://doc.qt.io/qt-5/signalsandslots.html)

[QCustomPlot](http://www.qcustomplot.com) is a widget that works with [Qt](https://www.qt.io) and excels at plotting and data visualization. It consists of two files (qcustomplot.cpp and qcustomplot.h). Plots are easy to code and have an extensive feature set for customization. Publication quality 2D plots, charts and graphs can be produced. Performance supports real-time data plotting and visualization applications. The [Real-Time Data Demo](http://www.qcustomplot.com/index.php/demos/realtimedatademo) was the basis for this example.

###Software Code
The software consists of two classes (Dialog and MainWindow). The Dialog class creates the GUI dialog with two buttons and a status field. This class is also responsible for searching and establishing a USB connection, and the timing functions for requesting and reading measurements from the pressure transducer. Only the Vendor ID (VID) is used to identify the USB Virtual port for the pressure transducer. However, it would be an easy modification to add the Product ID (PID) to the validation. Searching for a USB connection is conducted at half-second ticks and will continue indefinitely until a USB port satisfying the VID criteria is detected or the ‘Cancel Search’ button is clicked. After a USB connection is established and configured, clicking on the ‘Start Plotting’ button creates a signal the MainWindow plotting window to be created and shown.
A simple request/reply handshake protocol is used to get the measurements from the pressure transducer. The request is the text string “4\n” and the reply is received in the form of a text string, ranging from 1 to 5 characters in length, representing a number from -7500 to 7500 (representing -75 to +75 cm-H2O). For plotting fidelity and smoothness, 50 samples/sec (timer interval of 20 milliseconds) is an adequate measurement rate.The source files can be grouped into two classes (dialog and mainwindow) and a main.cpp file. For each class, there is a source file, a header file, and a graphical ui file. Finally, [Qt](https://www.qt.io) uses a project file.

###Standalone executable and DLL's
After the ‘Real-Time USB Pressure Monitor’ is compiled in a release configuration, it becomes an executable file (qt_tr_plotter.exe) but still requires a set of DLL files (located within the local [Qt](https://www.qt.io) directory structure) to be able to run as a standalone software program on Windows. For this specific software program, the relevant DLL files listed below should be included in the folder (note: qwindows.dll needs to reside in a subfolder 'platforms' along with the ‘Real-Time USB Pressure Monitor’ executable file:

- qt_tr_plotter.exe- libgss_s_dw2-1.dll- libstdc++-6.dll- libwinpthread-1.dll- Qt5Core.dll- Qt5Gui.dll- Qt5SerialPort.dll- Qt5Widgets.dll
- libEGL.dll
- platforms\qwindows.dll

###Summary
Using Qt Creator and QCustomPlot provides many benefits to creating GUI panels/dialogs and plotting options. It is easy to learn with the only unique concept being signals and slots to handle the events.

Read the [post](http://champlintechnologiesllc.com/real-time-plotting-using-qt-and-qcustomplot/) on [Champlin Technologies LLC](http://champlintechnologiesllc.com) for more info.
