
equals(QT_MAJOR_VERSION, 4) {
  QMAKE_CXXFLAGS += -std=c++11
  QMAKE_CXXFLAGS_RELEASE -= -std=gnu++98
  DEPENDPATH  += ..
  DEPENDPATH  += ../gui
  DEPENDPATH  += ../daq
  DEPENDPATH  += ../utils
  DEPENDPATH  += ../qcustomplot
  DEPENDPATH  += ../xpepeds
  QT += network
  }

equals(QT_MAJOR_VERSION, 5) {
  CONFIG += c++11
  VPATH  += ..
  VPATH  += ../gui
  VPATH  += ../daq
  VPATH  += ../utils
  VPATH  += ../qcustomplot
  VPATH  += ../xpepeds
  QT += network opengl
  }

INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ../gui
INCLUDEPATH += ../daq
INCLUDEPATH += ../utils
INCLUDEPATH += ../qcustomplot
INCLUDEPATH += ../xpepeds

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

HEADERS += qcustomplot.h
HEADERS += histogramUtils.h
HEADERS += pHistogramOptions.h
HEADERS += pHistogram.h
HEADERS += pCustomHistogramPlot.h
HEADERS += pHistogramPlot.h
HEADERS += pMap.h
HEADERS += pMapPlot.h
HEADERS += pCustomColorMapPlot.h

HEADERS += pDataBlock.h

HEADERS += pLoggerChannel.h
HEADERS += pLogger.h
HEADERS += pIOManager.h
HEADERS += pVariant.h
HEADERS += pOption.h
HEADERS += pOptionParser.h
HEADERS += pRunningStat.h
HEADERS += pChrono.h

HEADERS += pedestalsMap.h

HEADERS += pedFileIO.h
HEADERS += pedviewerPlotGrid.h
HEADERS += pedviewerWindow.h

SOURCES += xpolgui.cpp
SOURCES += xpoldetector.cpp
SOURCES += xpolfpga.cpp
SOURCES += xpedaqutils.cpp
SOURCES += xpedaqos.cpp
SOURCES += xpollog.cpp
SOURCES += xpolio.cpp
SOURCES += xpedaqheader.cpp

SOURCES += qcustomplot.cpp
SOURCES += histogramUtils.cpp
SOURCES += pHistogramOptions.cpp
SOURCES += pHistogram.cpp
SOURCES += pCustomHistogramPlot.cpp
SOURCES += pHistogramPlot.cpp
SOURCES += pMap.cpp
SOURCES += pMapPlot.cpp
SOURCES += pCustomColorMapPlot.cpp

SOURCES += pDataBlock.cpp

SOURCES += pLoggerChannel.cpp
SOURCES += pLogger.cpp
SOURCES += pIOManager.cpp
SOURCES += pVariant.cpp
SOURCES += pOption.cpp
SOURCES += pOptionParser.cpp
SOURCES += pRunningStat.cpp
SOURCES += pChrono.cpp

SOURCES += pedestalsMap.cpp

SOURCES += pedFileIO.cpp
SOURCES += pedviewerPlotGrid.cpp
SOURCES += pedviewerWindow.cpp

SOURCES += pedviewer.cpp

OBJECTS_DIR = ../build
MOC_DIR = ../moc
DESTDIR = ../bin

TARGET = pedviewer

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
