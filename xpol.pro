
QUICKUSB_VERSION = 2.15.2


INCLUDEPATH += ./gui
DEPENDPATH  += ./gui
INCLUDEPATH += ./daq
DEPENDPATH  += ./daq
INCLUDEPATH += ./utils
DEPENDPATH  += ./utils
INCLUDEPATH += ./quickusb-$${QUICKUSB_VERSION}
DEPENDPATH  += ./quickusb-$${QUICKUSB_VERSION}
  
unix {
ARCH = $$system(uname -m)
message(q-making for Linux $${ARCH}...)
LIBS += -L./quickusb-$${QUICKUSB_VERSION}/linux/lib/$${ARCH} -lusb -lquickusb
}

windows {
message(q-making for Windows...)
LIBS += -L./quickusb-$${QUICKUSB_VERSION}/windows -lQuickUsb
CONFIG += qt thread console
CONFIG -= debug_and_release
}

HEADERS += xpolgui.h
HEADERS += xpoldetector.h
HEADERS += xpolfpga.h
HEADERS += xpolutils.h
HEADERS += xpollog.h
HEADERS += xpolio.h
HEADERS += xpolenv.h

HEADERS += pQtGridLayoutWidget.h
HEADERS += pQtGroupBoxWidget.h
HEADERS += pQtCustomPushButton.h
HEADERS += pQtCustomTextLabel.h
HEADERS += pQtCustomTabWidget.h
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
HEADERS += pMainWindow.h

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
HEADERS += pHeaderFile.h

HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h
HEADERS += pUdpSender.h


SOURCES += xpolgui.cpp
SOURCES += xpoldetector.cpp
SOURCES += xpolfpga.cpp
SOURCES += xpolutils.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp
SOURCES += xpolenv.cpp

SOURCES += pQtGridLayoutWidget.cpp
SOURCES += pQtGroupBoxWidget.cpp
SOURCES += pQtCustomPushButton.cpp
SOURCES += pQtCustomTextLabel.cpp
SOURCES += pQtCustomTabWidget.cpp
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
SOURCES += pMainWindow.cpp

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
SOURCES += pHeaderFile.cpp

SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += pUdpSender.cpp


SOURCES += main.cpp

OBJECTS_DIR = build
MOC_DIR = moc

QT += network

TARGET = xpol

unix {
  QMAKE_CLEAN += ./gui/*~
  QMAKE_CLEAN += ./daq/*~
  QMAKE_CLEAN += ./quickusb-$${QUICKUSB_VERSION}/*~
  QMAKE_CLEAN += ./utils/*~
  QMAKE_CLEAN += $$TARGET
}

windows {
  QMAKE_CLEAN += xpol.exe
}

message(Done.)
