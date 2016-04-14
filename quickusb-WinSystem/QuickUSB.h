/*=============================================================================
 Title        : QuickUSB.c
 Description  : QuickUSB QUSB2 Module API
 Notes        : None
 History      :

 Copyright © 2003 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real, Suite E,
 Goleta, CA  93117.

 Bitwise Systems
 6489 Calle Real, Suite E
 Santa Barbara, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

 $History: QuickUSB.h $
// 
// *****************  Version 35  *****************
// User: Bob Jacobson Date: 3/09/06    Time: 11:14a
// Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsb
// Bump version to 2.11.4
// Add new I2C error status values.
// Mantis bugs 12 and 13
// 
// *****************  Version 34  *****************
// User: Blake Henry  Date: 3/09/06    Time: 7:49a
// Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsb
// Removed all documentation from .H file
// 
// *****************  Version 33  *****************
// User: Bob Jacobson Date: 2/08/06    Time: 5:56p
// Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagQT
// Update for QuickUSB Version 2.11.
// 
// *****************  Version 31  *****************
// User: Bob Jacobson Date: 10/18/05   Time: 2:45p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Bump DLL version number.
// Remove QuickUSB timer functions.
// Move some proprietary #defines out of quickusb.h.
// Add #defines for all QuickUSB settings.
// 
// *****************  Version 30  *****************
// User: Bob Jacobson Date: 10/11/05   Time: 11:50a
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Add new API functions QuickUsbGetSpeedCode and QuickUsbGetChipCode.
// 
// *****************  Version 29  *****************
// User: Bob Jacobson Date: 10/06/05   Time: 2:21p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Rewrite QuickUsbGetFirmwareVersion to use major, minor, and build
// versions, instead of a single version.
// 
// *****************  Version 28  *****************
// User: Bob Jacobson Date: 10/06/05   Time: 11:10a
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// In FindModules, look for Bitwise in string 6 in addition to string 1.
// Add new API function, QuickUsbGetFirmwareVersion, which returns the
// DID.
// Bump DLL version number.
// 
// *****************  Version 27  *****************
// User: Bob Jacobson Date: 9/21/05    Time: 5:23p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Add API functions to read and write defaults.
// Bump version to 2.10.1
// 
// *****************  Version 26  *****************
// User: Bob Jacobson Date: 9/19/05    Time: 2:57p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Change the 3rd argument of QuickUsbGetStringDescriptor back to char *
// from unsigned char *.
// 
// *****************  Version 25  *****************
// User: Bob Jacobson Date: 8/01/05    Time: 3:18p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Async I/O fixes.
// 
// *****************  Version 24  *****************
// User: Bob Jacobson Date: 7/25/05    Time: 6:23p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Changes to allow immediate return from QuickUsbAsyncWait
// 
// *****************  Version 23  *****************
// User: Eugenia Kaplun Date: 6/29/05    Time: 6:27p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Too many changes to list, but they're all good.
// 
// *****************  Version 22  *****************
// User: Blake Henry  Date: 11/03/04   Time: 6:57p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Fixed the QuickUsbFindModules bug that closes open ports
// 
// *****************  Version 20  *****************
// User: Blake Henry  Date: 7/26/04    Time: 3:17p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Fixed FindModules problem that set deviceHandle to 0
// Fixed 64MB transfer problem for non-async operation
// 
// *****************  Version 1  *****************
// User: Jay Towson   Date: 3/30/04    Time: 2:02p
// Created in $/Projects/Bitwise/QuickUSB/Software/Driver/QuickUsb
// 
// *****************  Version 10  *****************
// User: Blake Henry  Date: 2/17/04    Time: 6:39p
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// First cut of asynchronous DLL code
// 
// *****************  Version 9  *****************
// User: Blake Henry  Date: 8/21/03    Time: 9:00a
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Added timer constants
 
 *****************  Version 8  *****************
 User: Blake Henry  Date: 8/07/03    Time: 7:13p
 Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
 Added timer support
 Bumped rev to 2.00
 
 *****************  Version 7  *****************
 User: Alon Dakik   Date: 8/07/03    Time: 8:32a
 Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
 Changed some of the prototypes from having a char * parameter to
 unsigned char *.
 
 Also added a single line "bytesReceived = length" to allow for longer
 string descriptors
 
 *****************  Version 5  *****************
 User: Blake Henry  Date: 6/11/03    Time: 9:37a
 Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
 Fixed header to correctly display the History

=============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WIN32__
#include <windows.h>
#include <w32api.h>
#endif

#ifdef __linux__
#include <stdio.h>
#include "usb.h"
#include "string.h"
#define WINAPI
#define HANDLE  usb_dev_handle *
#endif

// QuickUSB String Descriptor Indices
#define QUICKUSB_MAKE                           1
#define QUICKUSB_MODEL                          2
#define QUICKUSB_SERIAL                         3


// QuickUSB settings
#define SETTING_EP26CONFIG                      0
#define SETTING_WORDWIDE                        1
#define SETTING_DATAADDRESS                     2
#define SETTING_FIFO_CONFIG                     3
#define SETTING_FPGATYPE                        4
#define SETTING_CPUCONFIG                       5
#define SETTING_SPICONFIG                       6
#define SETTING_SLAVEFIFOFLAGS                  7
#define SETTING_I2CTL                           8
#define SETTING_PORTA                           9
#define SETTING_PORTB                           10
#define SETTING_PORTC                           11
#define SETTING_PORTD                           12
#define SETTING_PORTE                           13
#define SETTING_PORTACCFG                       14
#define SETTING_PINFLAGS                        15
#define SETTING_VERSIONSPEED                    17

// FPGA type settings
#define FPGATYPE_ALTERAPS                       0
#define FPGATYPE_XILINXSS                       1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  QuickUSB Error Codes
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define QUICKUSB_ERROR_NO_ERROR                 0
#define QUICKUSB_ERROR_OUT_OF_MEMORY            1     
#define QUICKUSB_ERROR_CANNOT_OPEN_MODULE       2
#define QUICKUSB_ERROR_CANNOT_FIND_DEVICE       3
#define QUICKUSB_ERROR_IOCTL_FAILED             4
#define QUICKUSB_ERROR_INVALID_PARAMETER        5
#define QUICKUSB_ERROR_TIMEOUT                  6
#define QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED   7
#define QUICKUSB_ERROR_I2C_BUS_ERROR            8
#define QUICKUSB_ERROR_I2C_NO_ACK               9
#define QUICKUSB_ERROR_I2C_SLAVE_WAIT           10
#define QUICKUSB_ERROR_I2C_TIMEOUT              11


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  External function declarations
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// General purpose calls
extern int WINAPI QuickUsbGetLastError(unsigned long *error);
extern int WINAPI QuickUsbFindModules(char *nameList,
                                      unsigned long length);
extern int WINAPI QuickUsbOpen(HANDLE *hDevice, 
                               char *deviceName);

extern int WINAPI QuickUsbClose(HANDLE hDevice);

extern int WINAPI QuickUsbGetStringDescriptor(HANDLE hDevice, 
                                              unsigned char index,
                                              char *buffer, 
                                              unsigned short length);

extern int WINAPI QuickUsbReadSetting(HANDLE hDevice, 
                                      unsigned short address, 
                                      unsigned short *value);
extern int WINAPI QuickUsbWriteSetting(HANDLE hDevice,
                                       unsigned short address, 
                                       unsigned short value);

extern int WINAPI QuickUsbStartFpgaConfiguration(HANDLE hDevice);

extern int WINAPI QuickUsbWriteFpgaData(HANDLE hDevice, 
                                        unsigned char *fpgadata, 
                                        unsigned long datalength);

extern int WINAPI QuickUsbIsFpgaConfigured(HANDLE hDevice, 
              	                          unsigned short *isConfigured);


extern int WINAPI QuickUsbReadCommand(HANDLE hDevice, 
                                      unsigned short address, 
                                      unsigned char *data, 
                                      unsigned short *length);
extern int WINAPI QuickUsbWriteCommand(HANDLE hDevice, 
                                       unsigned short address, 
                                       unsigned char *data, 
                                       unsigned short length);


extern int WINAPI QuickUsbReadData(HANDLE hDevice, 
                                   unsigned char *data, 
                                   unsigned long *length);

extern int WINAPI QuickUsbWriteData(HANDLE hDevice, 
                                    unsigned char *data, 
                                    unsigned long length);

extern int WINAPI QuickUsbReadPortDir(HANDLE hDevice, 
                                   unsigned short address, 
                                   unsigned char *data);

extern int WINAPI QuickUsbWritePortDir(HANDLE hDevice, 
                                    unsigned short address, 
                                    unsigned char data);

extern int WINAPI QuickUsbReadPort(HANDLE hDevice, 
                                   unsigned short address, 
                                   unsigned char *data, 
                                   unsigned short *length);

extern int WINAPI QuickUsbWritePort(HANDLE hDevice, 
                                    unsigned short address, 
                                    unsigned char *data, 
                                    unsigned short length);


extern int WINAPI QuickUsbSetRS232BaudRate(HANDLE hDevice, 
                                           unsigned long baudRate);

extern int WINAPI QuickUsbGetNumRS232(HANDLE hDevice, 
                                      unsigned char portNum, 
                                      unsigned short *length);

extern int WINAPI QuickUsbFlushRS232(HANDLE hDevice, 
                                     unsigned char portNum);

extern int WINAPI QuickUsbReadRS232(HANDLE hDevice, 
                                    unsigned char portNum, 
                                    unsigned char *data, 
                                    unsigned short *length);

extern int WINAPI QuickUsbWriteRS232(HANDLE hDevice, 
                                     unsigned char portNum, 
                                     unsigned char *data, 
                                     unsigned short length);

extern int WINAPI QuickUsbReadSpi(HANDLE hDevice, 
                                  unsigned char portNum,
                                  unsigned char *data, 
                                  unsigned short *length);

extern int WINAPI QuickUsbWriteSpi(HANDLE hDevice, 
                                   unsigned char portNum,
                                   unsigned char *data, 
                                   unsigned short length);

extern int WINAPI QuickUsbWriteReadSpi(HANDLE hDevice, 
                                       unsigned char portNum, 
                                       unsigned char *data, 
                                       unsigned short length);

extern int WINAPI QuickUsbReadI2C(HANDLE hDevice, 
                                  unsigned short address, 
                                  unsigned char *data, 
                                  unsigned short *length);

extern int WINAPI QuickUsbWriteI2C(HANDLE hDevice, 
                                   unsigned short address, 
                                   unsigned char *data,
                                   unsigned short length);

extern int WINAPI QuickUsbSetTimeout(HANDLE hDevice,
                                     unsigned long timeOut);

extern int WINAPI QuickUsbGetDriverVersion(unsigned short * MajorDriverVersion,
                                           unsigned short * MinorDriverVersion,
                                           unsigned short * BuildDriverVersion);

extern int WINAPI QuickUsbGetDllVersion(unsigned short * MajorDllVersion,
                                        unsigned short * MinorDllVersion,
                                        unsigned short * BuildDllVersion);

extern int WINAPI QuickUsbGetFirmwareVersion(HANDLE hDevice, 
                                             unsigned short * MajorFirmwareVersion,
                                             unsigned short * MinorFirmwareVersion,
                                             unsigned short * BuildFirmwareVersion);

extern int WINAPI QuickUsbWriteDataAsync(HANDLE hDevice, 
                                         unsigned char *data, 
                                         unsigned long length, 
                                         unsigned char *transaction);

extern int WINAPI QuickUsbReadDataAsync(HANDLE hDevice, 
                                        unsigned char *data, 
                                        unsigned long *length, 
                                        unsigned char *transaction);

extern int WINAPI QuickUsbAsyncWait(HANDLE hDevice, 
                                    unsigned long *bytecount,
                                    unsigned char transaction,
                                    unsigned char immediate);

extern int WINAPI QuickUsbReadDefault(HANDLE hDevice,
                                      unsigned short address, 
                                      unsigned short *data);

extern int WINAPI QuickUsbWriteDefault(HANDLE hDevice,
                                       unsigned short address, 
                                       unsigned short data);

#ifdef __cplusplus
}
#endif
