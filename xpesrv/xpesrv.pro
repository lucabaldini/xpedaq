
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
  QT += network opengl core
  }

INCLUDEPATH += ..
INCLUDEPATH += ../gui
INCLUDEPATH += ../daq
INCLUDEPATH += ../utils

include(../quickusb-2.15.2/quickusb.pro)

windows {
  CONFIG += qt thread console
}

HEADERS += __version__.h
HEADERS += xpolgui.h
HEADERS += xpollog.h
HEADERS += xpolio.h
HEADERS += xpedaqos.h
HEADERS += xpedaqutils.h
HEADERS += xpoldetector.h

HEADERS += xServer.h
HEADERS += xServerGui.h
HEADERS += xpesrvWindow.h

HEADERS += pQtMainWindowGui.h
HEADERS += pQtGridLayoutWidget.h
HEADERS += pQtGroupBoxWidget.h
HEADERS += pQtCustomPushButton.h
HEADERS += pQtTransportBarGui.h
HEADERS += pTransportBar.h

HEADERS += pUdpSender.h
HEADERS += pDataBlock.h
HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h
HEADERS += pQtCustomTextLabel.h
HEADERS += pChrono.h

SOURCES += xpolgui.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp
SOURCES += xpedaqos.cpp
SOURCES += xpedaqutils.cpp
SOURCES += xpoldetector.cpp

SOURCES += xServer.cpp
SOURCES += xServerGui.cpp
SOURCES += xpesrvWindow.cpp

SOURCES += pQtMainWindowGui.cpp
SOURCES += pQtGridLayoutWidget.cpp
SOURCES += pQtCustomPushButton.cpp
SOURCES += pQtGroupBoxWidget.cpp
SOURCES += pQtTransportBarGui.cpp
SOURCES += pTransportBar.cpp
SOURCES += pUdpSender.cpp
SOURCES += pDataBlock.cpp

SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += pQtCustomTextLabel.cpp
SOURCES += pChrono.cpp

SOURCES += xpesrv.cpp


OBJECTS_DIR = ../build
MOC_DIR = ../moc
DESTDIR = ../bin

TARGET = xpesrv

