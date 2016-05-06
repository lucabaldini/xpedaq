
equals(QT_MAJOR_VERSION, 4) {
  DEPENDPATH  += ..
  DEPENDPATH  += ../gui
  DEPENDPATH  += ../daq
  DEPENDPATH  += ../utils
  DEPENDPATH  += ../qcustomplot
  QT += network
  }

equals(QT_MAJOR_VERSION, 5) {
  VPATH  += ..
  VPATH  += ../gui
  VPATH  += ../daq
  VPATH  += ../utils
  VPATH  += ../qcustomplot
  QT += network opengl
  }

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

HEADERS += pQtGridLayoutWidget.h
HEADERS += pQtGroupBoxWidget.h
HEADERS += pQtCustomPushButton.h
HEADERS += pQtCustomTextLabel.h
HEADERS += pQtTransportBarGui.h
HEADERS += pTransportBar.h
HEADERS += pQtDisplayGui.h
HEADERS += pDaqDisplay.h
HEADERS += pMessageDisplay.h
HEADERS += pQtCustomTab.h
HEADERS += pReadoutModeTab.h
HEADERS += pThresholdSettingTab.h
HEADERS += pAdvancedSettingsTab.h
HEADERS += pTriggerSettingTab.h
HEADERS += pProbeSettingTab.h
HEADERS += pUsbControlTab.h
HEADERS += pUserPreferencesTab.h
HEADERS += pQtMainWindowGui.h
HEADERS += pAcquisitionWindow.h
HEADERS += xpepedsWindow.h
HEADERS += pDisplayWindow.h
HEADERS += pedestalsMap.h

HEADERS += pFiniteStateMachine.h
HEADERS += pDataCollector.h
HEADERS += pRunController.h
HEADERS += pDataFIFO.h
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


SOURCES += xpolgui.cpp
SOURCES += xpoldetector.cpp
SOURCES += xpolfpga.cpp
SOURCES += xpedaqutils.cpp
SOURCES += xpedaqos.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp

SOURCES += qcustomplot.cpp

SOURCES += pQtGridLayoutWidget.cpp
SOURCES += pQtGroupBoxWidget.cpp
SOURCES += pQtCustomPushButton.cpp
SOURCES += pQtCustomTextLabel.cpp
SOURCES += pQtTransportBarGui.cpp
SOURCES += pTransportBar.cpp
SOURCES += pQtDisplayGui.cpp
SOURCES += pDaqDisplay.cpp
SOURCES += pMessageDisplay.cpp
SOURCES += pQtCustomTab.cpp
SOURCES += pReadoutModeTab.cpp
SOURCES += pThresholdSettingTab.cpp
SOURCES += pAdvancedSettingsTab.cpp
SOURCES += pTriggerSettingTab.cpp
SOURCES += pProbeSettingTab.cpp
SOURCES += pUsbControlTab.cpp
SOURCES += pUserPreferencesTab.cpp
SOURCES += pQtMainWindowGui.cpp
SOURCES += pAcquisitionWindow.cpp
SOURCES += xpepedsWindow.cpp
SOURCES += pDisplayWindow.cpp
SOURCES += pedestalsMap.cpp

SOURCES += pFiniteStateMachine.cpp
SOURCES += pDataCollector.cpp
SOURCES += pRunController.cpp
SOURCES += pDataFIFO.cpp
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

SOURCES += xpepeds.cpp

OBJECTS_DIR = build
MOC_DIR = moc

TARGET = xpepeds
DESTDIR = ../bin

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
