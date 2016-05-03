/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/

#ifndef PUSBCONTROLLER_H
#define PUSBCONTROLLER_H

#include <iostream>
#include <string>
#include <sstream>

#include "xpollog.h"
#include "CQuickUsb.h"
#include "QuickUSB.h"

#define WORDWIDE	1	//deprecated use SETTING_WORDWIDE
#define DATAADDRESS	2 	//deprecated use SETTING_DATAADDRESS
#define FIFO_MODE	3	//deprecated use SETTING_FIFO_CONFIG

#define WORDWIDE_SETT			1	//1= 16 bits, 0=8 bits
#define DATAADDRESS_SETT		0x4000	//address bus disabled B14=1 B15=1 mean don't increment
#define FIFO_MODE_SETT			0xE7	//MAster Mode:OxFA Slave Mode:0xf3   asynchslave 0xfB slave synch
#define PIXELS_TO_READ			13200
#define NWORDS				PIXELS_TO_READ*8
#define TIMEOUTMS			20000L
#define SRAM_DIM			262144	//2^18 //on board SRAm dimension
#define XPOL_FULL_FRAME_DIM		106496	//rounded to 104 1024 data blocks
//#define XPOL_DOUBLE_FULL_FRAME_DIM 	211968 	// deprecated, now is xpoldetector::kLargeBufferSize
//#define SHORT_BUFFER_DIM		5000    // deprecated, now is xpoldetector::kSmallBufferSize
#define N_SRAM_EVTS			2	//numberof frames stored in SRAM

#define PORTA		0
#define PORTB		1
#define PORTC		2
#define PORTD		3
#define PORTE		4
#define ALLINPUT	0x00
#define ALLOUTPUT	0Xff

#define STATUS_REG			0
#define X2NPIXEL_LSB_REG		0x01
#define X2NPIXEL_MSB_REG		0x02
#define TOXPOL_MSB_REG			0X03
#define TOXPOL_MID_REG			0X04
#define TOXPOL_LSB_REG			0X05
#define XPOL_SI_CNT_REG			0X06
#define XPOL_SO_SEL_REG			0x07
#define XPOL_SIGNAL_REG			0x08
#define XPOL_MIN_WIN_DIM_REG	0x09
#define XPOL_WPULSE_REG			0xa
#define XPOL_DISPIX_REG			0xb
#define XPOL_RDNGS_N_REG		13
#define XPOL_DANGEROUS_REG 		0x07

#define TRIGG_CNT_WINDOW_MSB_REG	0xd
#define TRIGG_CNT_WINDOW_LSB_REG	0xe
#define TRIGG_CNT_STATUS_REG		0xf
#define TRIGG_CNT_LOAD			0x1
#define TRIGG_CNT_RESET			0x2

#define RSEL				0x01
#define RECV				0x02
#define WSEL				0x04
#define SEND				0x08
#define reset_XPOLSI			0x10 
#define DATASEL				0x01


#define XPM_OUT_DATA_SEL_REG	0xd
#define XPM_AD2_DATA		000  //serial output interface channel selectors
#define XPM_CH1_DATA		001
#define XPM_CH2_DATA		002
#define XPM_ACQ_RATE		003
#define XPM_STATUS_REG		000
#define XPM_DACWORD_REG		001
#define XPM_MEM_MNGR_REG	002
#define XPM_PROBESPARE3_REG	0x3
#define XPM_PROBESPARE2_REG	0x4
#define XPM_PROBEEVENT_REG	0x5
#define XPM_NLOOP_LSW_REG	0x6
#define XPM_NLOOP_MSB_REG	0x7
#define XPM_MISC_SEL_REG	0x8
#define XPM_TIADSCFG_REG	0x9
#define XPM_TRG_CNT_STATUS_REG	0xA
#define XPM_TRG_CNT_STATUS_DATA 0x0
#define ADS_SI_RST		0x100
#define ADS_SI_SND		0x200
#define ADS_DESKEW_TP		0x4
#define ADS_SYNC_TP		0x8
//#define SMALL_BUFFER		0x1 // Deprecated, now is pXpolFpga.M_bufferMode

#define P_TriggWindow		0
#define P_ANAresetbar		1	
#define P_B14			2
#define P_MH			3	
#define P_DataReady		4	
#define P_USB_DATASENT		5
#define P_ACQWINDIM		6	
#define P_MinMaxRdone		7	
#define P_event_proc		8	
#define P_evebtacqrd		9	
#define P_SEQMOEn		0xA
#define P_SEQMWEn		0xB
#define P_SEQMCEn		0XC
#define P_BSEQMCEn		0XD
#define P_BSEQMOEn		0XE
#define P_BSEQMWEn		0XF
#define P_SEQ_RUN		0X10
#define P_SEQ_RST		0X11
#define P_USBSEND		0x12
#define P_XPSDOUT		0x13
#define P_XPSCLK		0x14
#define P_XPRDMINMAX		0x15
#define P_IFCLK			0x16
#define P_RDY1			0x17
#define P_PA7			0x18
#define P_ACQUIRE		0x19
#define P_ch_cnt_clk		0x1a
#define P_frame_cnt_clk		0x1b
#define P_mem_cnt_clk		0x1c
#define P_ADCLK			0x1d
#define P_HVRESET		0x1e
#define P_ADCRESET		0x1f

#define AD2_RUN			001
#define AD2_STOP		0xfffe
#define AD2_LSB			763E-6 //volts
#define DAC_LSB			806E-6 //volts				
#define TODAC1			0x0002
#define TODAC2			0x0004
#define SENDDAC			0x0008
#define XPOLAUTRIGGH		0x0010
#define XPOLAUTRIGGL		0xFFEF
#define	RSTDACSI			0xfff7
#define SINGLEBUFF_FFRAME	0x0
#define DOUBLEBUFF_FFRAME	0x1
#define WINDOWED			0x2
#define FULLFRAME_MH		0x84
#define WINDOWED_INJ		0x2CE
#define WINDOWED_EVT		0x28E//runB disabled will stop he write pulse

#define RESET			0x010
#define RUN			0x008
#define NEW			0x001
#define WRN_RD			0x004
#define AUTOTRIGG		0x002
#define NORMAL			0x0
#define DESKEW			0x4
#define SYNC			0x8
#define SO_EN			9 //actually S4
#define SCLK			10
#define SDATA			11
#define MEM_ADD_CNT_CLK		12
#define MEM_ADD_CNT_RST		13
#define UC_BUSMASTER		14 
#define SPARE0			15 
#define ADC_CONF_SEND		16 //actually SPARE4 Pin
#define SPARE3			17
#define SPARE2			18 
#define SPARE1			19

#define EP6FULLFLG	0x10
#define EP6EMPTYFLG	0x200 


class pUsbController : public QObject, public CQuickUsb
{

  Q_OBJECT

 public:

  pUsbController();
  ~pUsbController();
  unsigned long connect();

  std::string deviceName();
  std::string driverVersion();
  std::string dllVersion();
  std::string firmwareVersion();
  
  int setup();
  int readUsbSettings();
  int writeUsbSettings();
  int startSequencer();
  int stopSequencer();
  int resetSequencer();
  int flushQUsbFIFO();
  unsigned long lastErrorCode();
  void errorSummary(unsigned long errorCode);
  int readSetting(unsigned short address, unsigned short *value);
  int readData(unsigned char *data, unsigned long *length);
  int writeSetting(unsigned short address, unsigned short value);
  int writePortDir(unsigned short address, unsigned char data);
  int writePort(unsigned short address, unsigned char *data,
		unsigned short length);
  unsigned long timeout() const {return m_timeout;}
  int setTimeout(unsigned long timeout);
  static std::string errorDescription(unsigned long errorCode);
  static std::string errorResolution(unsigned long errorCode);


 signals:

  void connected(QString deviceName, QString driverVersion, QString dllVersion,
		 QString firmwareVersion);
  void quickusbError(unsigned long errorCode);

    
 protected:

 private:

  /// \brief Private bookkeping member to keep track of the current timeout.
  unsigned long m_timeout;
  
};

#endif
