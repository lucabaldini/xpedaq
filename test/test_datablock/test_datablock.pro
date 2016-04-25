INCLUDEPATH += ../../utils
DEPENDPATH  += ../../utils
INCLUDEPATH += ../../daq
DEPENDPATH  += ../../daq

QUICKUSB_VERSION = 2.15.2

INCLUDEPATH += ../../quickusb-$${QUICKUSB_VERSION}
DEPENDPATH  += ../../quickusb-$${QUICKUSB_VERSION}  

unix {
ARCH = $$system(uname -m)
message(q-making for Linux $${ARCH}...)
LIBS += -L../../quickusb-$${QUICKUSB_VERSION}/linux/lib/$${ARCH} -lusb -lquickusb
}

windows {
message(q-making for Windows...)
LIBS += -L../../quickusb-$${QUICKUSB_VERSION}/windows -lQuickUsb
CONFIG += qt thread console
CONFIG -= debug_and_release
}


windows {
  CONFIG += qt console
}

HEADERS += xpollog.h
HEADERS += xpolio.h
HEADERS += xpolenv.h
HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h
HEADERS += pDataBlock.h
HEADERS += pUsbController.h
HEADERS += CQuickUsb.h

SOURCES += xpollog.cpp
SOURCES += xpolio.cpp
SOURCES += xpolenv.cpp
SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += pDataBlock.cpp
SOURCES += pUsbController.cpp
SOURCES += CQuickUsb.cpp

SOURCES += test_datablock.cpp

TARGET = test_datablock

QMAKE_CLEAN += $$TARGET
