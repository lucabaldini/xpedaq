
QUICKUSB_VERSION=2.15.2
ARCH=$(uname -m)

qusb_install_kernel_module()
{
    # Compile the driver.
    KM_INSTALL_PATH=/lib/modules/$(uname -r)/kernel/drivers/usb/misc
    if [ -f $KM_INSTALL_PATH/qusb_lnx.ko ];
    then
	:
    else
	echo "Compiling kernel..."
	cd driver
	make
	echo "Copying kernel module..."
	sudo cp $(uname -m)/qusb_lnx.ko $KM_INSTALL_PATH
	cd -
    fi
}

qusb_link_libs()
{
    # Create the links to the libraries if they don't exist.
    if [ -f lib/$ARCH/libquickusb.so ];
    then
	:
    else
	cd lib/$ARCH/
	echo "Creating soft link in" $(pwd) "..."
	ln -s libquickusb.so.$QUICKUSB_VERSION libquickusb.so
	cd -
    fi
}

qusb_copy_rule_file()
{
    # Copy the rule file.
    if [ -f /etc/udev/rules.d/90-qusb.rules ];
    then
	:
    else
	echo "Copying 90-qusb.rules to /etc/udev/rules.d/..."
	sudo cp 90-qusb.rules /etc/udev/rules.d/
    fi
}

qusb_copy_usb_scripts()
{
    # Copy the USB script.
    if [ -f /usr/local/bin/qusb_script.sh ];
    then
	:
    else
	echo "Copying qusb_script.sh in /usr/local/bin..."
	sudo cp qusb_script.sh /usr/local/bin
    fi
}
