obj-m	:= qusb_lnx.o
qusb_lnx-y := driver.o control.o bulk.o sg.o

INSTALL_PATH = /lib/modules/$(shell uname -r)/kernel/drivers/usb/misc
KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD=$(shell pwd)
ECHO=@echo
REDIRECT=> /dev/null
MV=mv
RM=rm
CP=cp
INSMOD=/sbin/insmod
RMMOD=/sbin/rmmod

DISP_CC  = "Building"
DISP_RM  = "Cleaning up"
DISP_INS = "Inserting kernel object"
DISP_REM = "Removing kernel object"
OUTDIR   = $(shell uname -m)

all: $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(ECHO) $(DISP_CC) "$(OUTDIR)/$(obj-m)"
	@$(MAKE) -C $(KERNELDIR) M=$(PWD) modules $(REDIRECT)
	@$(MV) qusb_lnx.ko $(OUTDIR)
	$(ECHO) $(DISP_RM)
	@$(RM) -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

install: all
	$(ECHO) $(DISP_INS) "$(PROGRAM)"
	@$(INSMOD) $(OUTDIR)/qusb_lnx.ko
	@$(CP) $(OUTDIR)/qusb_lnx.ko $(INSTALL_PATH)/qusb_lnx.ko

reinstall: all
	$(ECHO) $(DISP_REM) "$(PROGRAM)"
	@$(RMMOD) qusb_lnx
	$(ECHO) $(DISP_INS) "$(PROGRAM)"
	@$(INSMOD) $(OUTDIR)/qusb_lnx.ko
	@$(CP) $(OUTDIR)/qusb_lnx.ko $(INSTALL_PATH)/qusb_lnx.ko

clean:
	$(ECHO) $(DISP_RM)
	@$(RM) -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions $(shell uname -m)
