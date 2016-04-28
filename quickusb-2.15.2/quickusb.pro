QUICKUSB_VERSION = 2.15.2

equals(QT_MAJOR_VERSION, 4) {
  DEPENDPATH  += ../quickusb-$${QUICKUSB_VERSION}
}

equals(QT_MAJOR_VERSION, 5) {
  VPATH  += ../quickusb-$${QUICKUSB_VERSION}
}

INCLUDEPATH += ../quickusb-$${QUICKUSB_VERSION}

unix {
  ARCH = $$system(uname -m)
  message(q-making for Linux $${ARCH}...)
  LIBS += -L../quickusb-$${QUICKUSB_VERSION}/linux/lib/$${ARCH} -lusb -lquickusb
}

windows {
  message(q-making for Windows...)
  LIBS += -L../quickusb-$${QUICKUSB_VERSION}/windows -lQuickUsb
  CONFIG += qt thread console
  CONFIG -= debug_and_release
}
