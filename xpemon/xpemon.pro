
equals(QT_MAJOR_VERSION, 4) {
  QMAKE_CXXFLAGS += -std=c++11
  QMAKE_CXXFLAGS_RELEASE -= -std=gnu++98
  DEPENDPATH  += ..
  DEPENDPATH  += ../gui
  DEPENDPATH  += ../daq
  DEPENDPATH  += ../utils
  DEPENDPATH  += ../qcustomplot
  QT += network
  }

equals(QT_MAJOR_VERSION, 5) {
  CONFIG += c++11
  VPATH  += ..
  VPATH  += ../gui
  VPATH  += ../daq
  VPATH  += ../utils
  VPATH  += ../qcustomplot
  QT += network opengl
  }

INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ../gui
INCLUDEPATH += ../daq
INCLUDEPATH += ../utils
INCLUDEPATH  += ../qcustomplot

include(../quickusb-2.15.2/quickusb.pro)

HEADERS += __version__.h
HEADERS += xpolgui.h
HEADERS += xpoldetector.h
HEADERS += xpolfpga.h
HEADERS += xpedaqutils.h
HEADERS += xpedaqos.h
HEADERS += xpollog.h
HEADERS += xpolio.h

HEADERS += qcustomplot.h
HEADERS += histogramUtils.h
HEADERS += pHistogramOptions.h
HEADERS += pHistogram.h
HEADERS += pCustomHistogramPlot.h
HEADERS += pHistogramPlot.h
HEADERS += pMap.h
HEADERS += pMapPlot.h
HEADERS += pCustomColorMapPlot.h
HEADERS += pHexagon.h
HEADERS += pHexagonMatrix.h
HEADERS += pEventDisplay.h
HEADERS += pEvent.h
HEADERS += pCluster.h
 
HEADERS += pQtGridLayoutWidget.h
HEADERS += pQtGroupBoxWidget.h
HEADERS += pQtCustomPushButton.h
HEADERS += pQtCustomTextLabel.h
HEADERS += pQtTransportBarGui.h
HEADERS += pTransportBar.h
HEADERS += pQtDisplayGui.h
HEADERS += pQtCustomTab.h
HEADERS += pQtMainWindowGui.h

HEADERS += pFiniteStateMachine.h
HEADERS += pDataBlock.h
HEADERS += CQuickUsb.h
HEADERS += pUsbController.h
HEADERS += pXpolFpga.h
HEADERS += pDetectorConfiguration.h
HEADERS += pUserPreferences.h
HEADERS += pTriggerMask.h

HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h
HEADERS += pUdpSender.h

HEADERS += xpemonWindow.h
HEADERS += pOptionBoxWidget.h
HEADERS += pEventReader.h
HEADERS += pInfoBoxWidget.h
HEADERS += pMonitorPreferences.h
HEADERS += xpemonPlotOptions.h
HEADERS += pMonitorTab.h
HEADERS += pEventDisplayTab.h
HEADERS += pHexagonCoordinates.h

SOURCES += xpolgui.cpp
SOURCES += xpoldetector.cpp
SOURCES += xpolfpga.cpp
SOURCES += xpedaqutils.cpp
SOURCES += xpedaqos.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp

SOURCES += qcustomplot.cpp
SOURCES += histogramUtils.cpp
SOURCES += pHistogramOptions.cpp
SOURCES += pHistogram.cpp
SOURCES += pCustomHistogramPlot.cpp
SOURCES += pHistogramPlot.cpp
SOURCES += pMap.cpp
SOURCES += pMapPlot.cpp
SOURCES += pCustomColorMapPlot.cpp
SOURCES += pHexagon.cpp
SOURCES += pHexagonMatrix.cpp
SOURCES += pEventDisplay.cpp

SOURCES += pQtGridLayoutWidget.cpp
SOURCES += pQtGroupBoxWidget.cpp
SOURCES += pQtCustomPushButton.cpp
SOURCES += pQtCustomTextLabel.cpp
SOURCES += pQtTransportBarGui.cpp
SOURCES += pTransportBar.cpp
SOURCES += pQtDisplayGui.cpp
SOURCES += pQtCustomTab.cpp
SOURCES += pQtMainWindowGui.cpp

SOURCES += pFiniteStateMachine.cpp
SOURCES += pDataBlock.cpp
SOURCES += CQuickUsb.cpp
SOURCES += pUsbController.cpp
SOURCES += pXpolFpga.cpp
SOURCES += pDetectorConfiguration.cpp
SOURCES += pUserPreferences.cpp
SOURCES += pTriggerMask.cpp

SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += pUdpSender.cpp

SOURCES += xpemonWindow.cpp
SOURCES += pOptionBoxWidget.cpp
SOURCES += pEventReader.cpp
SOURCES += pInfoBoxWidget.cpp
SOURCES += pMonitorPreferences.cpp
SOURCES += xpemonPlotOptions.cpp
SOURCES += pMonitorTab.cpp
SOURCES += pEventDisplayTab.cpp
SOURCES += pHexagonCoordinates.cpp
SOURCES += pCluster.cpp
SOURCES += pEvent.cpp

SOURCES += xpemon.cpp

OBJECTS_DIR = ../build
MOC_DIR = ../moc
DESTDIR = ../bin

TARGET = xpemon

unix {
  QMAKE_CLEAN += ./gui/*~
  QMAKE_CLEAN += ./daq/*~
  QMAKE_CLEAN += ./quickusb-$${QUICKUSB_VERSION}/*~
  QMAKE_CLEAN += ./utils/*~
  QMAKE_CLEAN += $$DESTDIR/$$TARGET
}

windows {  
  QMAKE_CLEAN += $$DESTDIR/$$TARGET.exe
}

message(Done.)
