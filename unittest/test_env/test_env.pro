INCLUDEPATH += ../../utils
DEPENDPATH  += ../../utils

windows {
  CONFIG += qt console
}

HEADERS += xpolenv.h

SOURCES += xpolenv.cpp
SOURCES += test_env.cpp

TARGET   = test_env

QMAKE_CLEAN += $$TARGET
