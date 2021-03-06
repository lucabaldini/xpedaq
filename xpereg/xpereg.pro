
equals(QT_MAJOR_VERSION, 4) {
  QMAKE_CXXFLAGS += -std=c++11
  QMAKE_CXXFLAGS_RELEASE -= -std=gnu++98
  DEPENDPATH  += ..
  DEPENDPATH  += ../gui
  DEPENDPATH  += ../daq
  DEPENDPATH  += ../utils
  QT += network
  }

equals(QT_MAJOR_VERSION, 5) {
  CONFIG += c++11
  VPATH  += ..
  VPATH  += ../gui
  VPATH  += ../daq
  VPATH  += ../utils
  QT += network opengl
  }

INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ../gui
INCLUDEPATH += ../daq
INCLUDEPATH += ../utils

include(../quickusb-2.15.2/quickusb.pro)

HEADERS += __version__.h
HEADERS += xpolgui.h
HEADERS += xpoldetector.h
HEADERS += xpolfpga.h
HEADERS += xpedaqutils.h
HEADERS += xpedaqos.h
HEADERS += xpollog.h
HEADERS += xpolio.h
HEADERS += xpedaqheader.h

HEADERS += pQtGridLayoutWidget.h
HEADERS += pQtGroupBoxWidget.h
HEADERS += pQtCustomPushButton.h
HEADERS += pQtCustomTextLabel.h
HEADERS += pQtTransportBarGui.h
HEADERS += pTransportBar.h
HEADERS += pCommentBoxWidget.h
HEADERS += pQtDisplayGui.h
HEADERS += pDaqDisplay.h
HEADERS += pDataDisplayWidget.h
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
HEADERS += xperegRegisterTab.h
HEADERS += xperegUserPreferencesTab.h

HEADERS += xperegWindow.h
HEADERS += xperegRunController.h

HEADERS += pFiniteStateMachine.h
HEADERS += pDataCollector.h
HEADERS += pRunController.h
HEADERS += pDataFIFO.h
HEADERS += pDataBlock.h
HEADERS += CQuickUsb.h
HEADERS += pUsbController.h
HEADERS += pXpolFpga.h
HEADERS += pXpolRegisterPoker.h
HEADERS += pDetectorConfiguration.h
HEADERS += pUserPreferences.h
HEADERS += xperegUserPreferences.h
HEADERS += pTriggerMask.h

HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h
HEADERS += pVariant.h
HEADERS += pOption.h
HEADERS += pOptionParser.h
HEADERS += pChrono.h

HEADERS += pUdpSender.h
HEADERS += pTriggerMask.h

SOURCES += xpolgui.cpp
SOURCES += xpoldetector.cpp
SOURCES += xpolfpga.cpp
SOURCES += xpedaqutils.cpp
SOURCES += xpedaqos.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp
SOURCES += xpedaqheader.cpp

SOURCES += pQtGridLayoutWidget.cpp
SOURCES += pQtGroupBoxWidget.cpp
SOURCES += pQtCustomPushButton.cpp
SOURCES += pQtCustomTextLabel.cpp
SOURCES += pQtTransportBarGui.cpp
SOURCES += pTransportBar.cpp
SOURCES += pCommentBoxWidget.cpp
SOURCES += pQtDisplayGui.cpp
SOURCES += pDaqDisplay.cpp
SOURCES += pDataDisplayWidget.cpp
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
SOURCES += xperegRegisterTab.cpp
SOURCES += xperegUserPreferencesTab.cpp

SOURCES += xperegWindow.cpp
SOURCES += xperegRunController.cpp

SOURCES += pFiniteStateMachine.cpp
SOURCES += pDataCollector.cpp
SOURCES += pRunController.cpp
SOURCES += pDataFIFO.cpp
SOURCES += pDataBlock.cpp
SOURCES += CQuickUsb.cpp
SOURCES += pUsbController.cpp
SOURCES += pXpolFpga.cpp
SOURCES += pXpolRegisterPoker.cpp
SOURCES += pDetectorConfiguration.cpp
SOURCES += pUserPreferences.cpp
SOURCES += xperegUserPreferences.cpp

SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += pVariant.cpp
SOURCES += pOption.cpp
SOURCES += pOptionParser.cpp
SOURCES += pChrono.cpp

SOURCES += pUdpSender.cpp
SOURCES += pTriggerMask.cpp

SOURCES += xpereg.cpp

OBJECTS_DIR = ../build
MOC_DIR = ../moc
DESTDIR = ../bin

TARGET = xpereg

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
