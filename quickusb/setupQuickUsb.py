#! /bin/env python

import sys
import commands
import os


## Look at the hotplug.readme file for more details.

## Need to copy quickusb and quickusb.usermap to /etc/hotplug/usb

def copyHotplugFiles():
    command = 'su -c "cp quickusb quickusb.usermap /etc/hotplug/usb/"'
    print 'Please enter the root password to execute:'
    print '> %s' % command
    os.system(command)

## And the user must be member of the usb group (which has in turn to exist).

def checkGroups():
    print 'Checking groups...'
    groupsFilePath = '/etc/group'
    usbGroupName = 'usb'
    command = 'cat %s |cut -d: -f1' % groupsFilePath
    groups = commands.getoutput(command).split('\n')
    if usbGroupName not in groups:
        print 'Could not find "%s" in %s...' % (usbGroupName, groupsFilePath)
        sys.exit('Please create the "%s" group.' % usbGroupName)
    command = 'groups'
    groups = commands.getoutput(command).split()
    if usbGroupName not in groups:
        print 'You do not seem to belong to the "%s" group...' % usbGroupName
        sys.exit('Please add yourself to the "%s" group.' % usbGroupName)
    print 'Done.'


if __name__ == '__main__':
    checkGroups()
    copyHotplugFiles()
