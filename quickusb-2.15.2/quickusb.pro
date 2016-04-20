QUICKUSB_VERSION = 2.15.2

INCLUDEPATH += ../quickusb-$${QUICKUSB_VERSION}
DEPENDPATH  += ../quickusb-$${QUICKUSB_VERSION}  

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
