
equals(QT_MAJOR_VERSION, 4) {
  DEPENDPATH  += ..
  DEPENDPATH  += ../gui
  DEPENDPATH  += ../daq
  DEPENDPATH  += ../utils
  QT += network
  }

equals(QT_MAJOR_VERSION, 5) {
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
HEADERS += xServer.h
HEADERS += xServerGui.h
HEADERS += pUdpSender.h
HEADERS += pDataBlock.h
HEADERS += xpollog.h
HEADERS += xpolio.h
HEADERS += xpedaqutils.h
HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h
HEADERS += pQtCustomTextLabel.h
HEADERS += xpoldetector.h
HEADERS += pEvent.h

SOURCES += xServer.cpp
SOURCES += xServerGui.cpp
SOURCES += pUdpSender.cpp
SOURCES += pDataBlock.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp
SOURCES += xpedaqutils.cpp
SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += pQtCustomTextLabel.cpp
SOURCES += xpesrv.cpp
SOURCES += xpoldetector.cpp
SOURCES += pEvent.cpp

OBJECTS_DIR = ../build
MOC_DIR = ../moc
DESTDIR = ../bin

TARGET = xpesrv

