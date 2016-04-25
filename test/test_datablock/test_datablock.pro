INCLUDEPATH += ../../utils
DEPENDPATH  += ../../utils
INCLUDEPATH += ../../daq
DEPENDPATH  += ../../daq

windows {
  CONFIG += qt console
}

HEADERS += pHeaderFile.h
HEADERS += xpollog.h
HEADERS += xpolio.h
HEADERS += xpolenv.h
HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h

SOURCES += pHeaderFile.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp
SOURCES += xpolenv.cpp
SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += test_datablock.cpp

TARGET   = test_datablock

QMAKE_CLEAN += ./test.dat
QMAKE_CLEAN += $$TARGET
