INCLUDEPATH += ..
INCLUDEPATH += ../utils
DEPENDPATH  += ../utils
INCLUDEPATH += ../daq
DEPENDPATH  += ../daq
INCLUDEPATH += ../gui
DEPENDPATH  += ../gui
INCLUDEPATH +=

include(../quickusb-2.15.2/quickusb.pro)

windows {
  CONFIG += qt thread console
}

HEADERS += xServer.h
HEADERS += xServerGui.h
HEADERS += pUdpSender.h
HEADERS += pDataBlock.h
HEADERS += xpollog.h
HEADERS += xpolio.h
HEADERS += xpolenv.h
HEADERS += xpedaqutils.h
HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h
HEADERS += pQtCustomTextLabel.h

SOURCES += xServer.cpp
SOURCES += xServerGui.cpp
SOURCES += pUdpSender.cpp
SOURCES += pDataBlock.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp
SOURCES += xpolenv.cpp
SOURCES += xpedaqutils.cpp
SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += pQtCustomTextLabel.cpp
SOURCES += xpesrv.cpp

QT += network

OBJECTS_DIR = build
MOC_DIR = moc

TARGET = xpesrv
DESTDIR = ../bin
