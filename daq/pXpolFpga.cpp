/***********************************************************************
Copyright (C) 2007, 2008 by Luca Baldini (luca.baldini@pi.infn.it),
Johan Bregeon, Massimo Minuti and Gloria Spandre.

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

#include "pXpolFpga.h"

pXpolFpga::pXpolFpga(pUsbController *usbController)
{
  m_usbController = usbController;
}


pXpolFpga::~pXpolFpga()
{
  
}


int pXpolFpga::resetSequencer()
{
  *xpollog::kInfo << "Resetting Sequencer..." << endline;	
  serialWrite(STATUS_REG,RESET);
  return 1;
}


void pXpolFpga::serialWrite(unsigned short REG_ADD,unsigned short regdata)
{
  unsigned char temp[26];
  unsigned short i;
  unsigned short data;
  data=((REG_ADD<<8)&0XF00)|regdata;
  for(i=0;i<24;i++) temp[i+1]=(i|0x4);
  temp[0]=1;
  temp[25]=1;
  for(i=0;i<24;i++){
    if (data & (0x1<<(i/2))){
      temp[i+1]|=0x02;	  // 00000010
    } else {
      temp[i+1]&=0x05;   // 00000101
    }
  }
  m_usbController->WritePort(PORTE,temp,26);
}


void pXpolFpga::mainSerialWrite(unsigned short REG_ADD,unsigned short regdata)
{
  unsigned char temp[50];
  unsigned short i;
  unsigned int data;
  data=(((unsigned int)REG_ADD<<16)&0XFF0000)|regdata;
  // the same scheme used for 12 bit interface is adapted on the PE[3,4,5]
  for(i=0;i<48;i++){
    temp[i+1]=(((i|0x4)<<3)&0xfff8);
  }
  temp[0]=1;
  temp[49]=1;
  for(i=0;i<48;i++){
    if (data & (0x1<<(i/2))){
      temp[i+1]|=0x10;	// 00010000
    } else {
      temp[i+1]&=0x28;	// 00101000
    }
  }
  m_usbController->WritePort(PORTE,temp,50);
}


/* Enable all the pixels to trigger.
 */
void pXpolFpga::enableAllPixels()
{
  *xpollog::kInfo << "Enabling all pixels to trigger... ";
  setupToDisablePixels();
  serialWrite(XPOL_SIGNAL_REG, 0x4);
  serialWrite(XPOL_SIGNAL_REG, 0x0);
  *xpollog::kInfo << "Done." << endline;
}


/* Mask a single pixel.
 */
void pXpolFpga::maskPixel(unsigned short x, unsigned short y)
{
  setupToDisablePixels();
  writeXpolAddressRegister(x, y);
  serialWrite(XPOL_DISPIX_REG, 0x1); //disable pixel
  serialWrite(XPOL_DISPIX_REG, 0x0);
}


/* Apply a trigger mask.
 */
void pXpolFpga::applyTriggerMask(pTriggerMask *trgMask)
{
  enableAllPixels();
  if (trgMask->size()) {
    //setupToDisablePixels();
    for (const auto &chan : *(trgMask->mask())) {
      *xpollog::kInfo << "Masking pixel <" << chan.first << ", " 
		      << chan.second << ">... " << endline;
      writeXpolAddressRegister(chan.first, chan.second);
      serialWrite(XPOL_DISPIX_REG, 0x1); //disable pixel
      serialWrite(XPOL_DISPIX_REG, 0x0);
    }
  }
}


/* In order to mask a pixel to the trigger XPOL must be configured in 
   READMODE 0 and WRITEMODE 0.
 */
void pXpolFpga::setupToDisablePixels()
{
  writeXpolConfigurationRegister(0x0);
}


//read a 16-bit word from fpga serial interface B
//PE[3] is SCLK, PE[7] is SO_EN, PE[6] is datain (output from fpga)
//MSb first!!!!!!
uint32_t pXpolFpga::mainSerialRead(unsigned short reg_address){
  unsigned char clkpulse_vect[3];
  unsigned char data;
  unsigned short length=1,i;
  unsigned dataread;
  //initialize signals
  mainSerialWrite((unsigned short)XPM_OUT_DATA_SEL_REG,(unsigned short)reg_address);	//this selects register to read
  clkpulse_vect[0]=0x00;// pe[7] to 1 and pe[3] to 0					//writing in the XPM_OUT_DATA_SEL_REG
  clkpulse_vect[1]=0x08;// pe[7] to 0 and pe[3] to 1
  clkpulse_vect[2]=0x00;// pe[7] to 1 and pe[3] to 0
  
  m_usbController->WritePort(PORTE,clkpulse_vect,3);	//a clock pulse while SO_EN is low
  //loads data in fpga's shift register	
  m_usbController->ReadPort(PORTE, &data,&length);	
  
  clkpulse_vect[0]=0x80;// pe[7] to 1 and pe[3] to 0
  clkpulse_vect[1]=0x88;// pe[7] to 1 and pe[3] to 1
  clkpulse_vect[2]=0x80;// pe[7] to 1 and pe[3] to 0
  
  dataread=0;
  for (i=0;i<16;i++){
    m_usbController->ReadPort(PORTE, &data,&length);
    if (data&0x40){
      dataread|=(0x00008000>>i);
    } else {
      dataread&=(0xffff7fff>>i);
    }
    //dataread|= (data<<(9-i))&(0x8000>>i); //shift to bring b[6] in b[15-i],clear all the other bit read from portE
    m_usbController->WritePort(PORTE,clkpulse_vect,3);//apply clock pulse
  }
  clkpulse_vect[0]=0;
  m_usbController->WritePort(PORTE,clkpulse_vect,1); // reset all lines
  return(dataread);
}


/* Write the XPOL configuration register.
 */
void pXpolFpga::writeXpolConfigurationRegister(unsigned short value)
{
  serialWrite(15, 0x0); // To send configuration to XPOL SIMODE must be 0.
  serialWrite(TOXPOL_MSB_REG, ((value & 0x3f) >> 4) & 0x03);
  serialWrite(TOXPOL_MID_REG, ((value & 0x3f) << 4) & 0xf0);
  serialWrite(XPOL_SI_CNT_REG, reset_XPOLSI);
  serialWrite(XPOL_SI_CNT_REG, 0);
  serialWrite(XPOL_SI_CNT_REG, WSEL);//WSEL=1 CONFIGURATION mode
  serialWrite(XPOL_SI_CNT_REG, SEND | WSEL);//WSEL=1 CONFIGURATION mode
  serialWrite(XPOL_SI_CNT_REG, 0);
}


void pXpolFpga::writeXpolAddressRegister(unsigned short X, unsigned short Y)
{
  serialWrite(15,0);
  serialWrite(TOXPOL_MSB_REG, (X>>7)&0x03);
  serialWrite(TOXPOL_MID_REG, ((X<<1)&0xfe)|((Y>>8)&0x1));
  serialWrite(TOXPOL_LSB_REG, Y&0xFF);
  serialWrite(XPOL_SI_CNT_REG, reset_XPOLSI);
  serialWrite(XPOL_SI_CNT_REG, 0);
  serialWrite(XPOL_SI_CNT_REG, SEND);//WSEL=0 address mode
  serialWrite(XPOL_SI_CNT_REG, 0);	
}


void pXpolFpga::readXpolAddressConfigurationRegisters(unsigned short &x,
						      unsigned short &y,
						      unsigned short &conf)
{
  serialWrite(15,0);
  serialWrite(XPOL_SI_CNT_REG, reset_XPOLSI);
  serialWrite(XPOL_SI_CNT_REG, 0);
  serialWrite(XPOL_SI_CNT_REG, RECV);
  serialWrite(XPOL_SI_CNT_REG, 0);

  unsigned short lowWord = mainSerialRead(XPM_RBACK_LSW);
  unsigned short midWord = mainSerialRead(XPM_RBACK_MIDW);
  unsigned short highWord = mainSerialRead(XPM_RBACK_MSW);

  x = lowWord >> 9;
  x |= ((midWord & 0x0003) << 7);
  y = lowWord & 0x01FF;
  conf = ((highWord & 0x000f) << 2) ;
  conf |= ((midWord >> 14) & 0x03);
}


uint32_t pXpolFpga::readErrorCode()
{
  return mainSerialRead(XPM_ERROR_CODE);
}


uint32_t pXpolFpga::readEventCounter()
{
  return mainSerialRead(XPM_EVT_COUNTER_LSW) +
    (mainSerialRead(XPM_EVT_COUNTER_MSW) << 16);
}


uint32_t pXpolFpga::readRejectedEventCounterWllim()
{
  return mainSerialRead(XPM_WLLIM_REJ_COUNTER_LSW) +
    (mainSerialRead(XPM_WLLIM_REJ_COUNTER_MSW) << 16);
}


uint32_t pXpolFpga::readRejectedEventCounterWulim()
{
  return mainSerialRead(XPM_WULIM_REJ_COUNTER_LSW) +
    (mainSerialRead(XPM_WULIM_REJ_COUNTER_MSW) << 16);
}


void pXpolFpga::readReadoutStatistics(uint32_t &evtCounter,
				      uint32_t &rejEvtCounterWllim,
				      uint32_t &rejEvtCounterWulim )
{
  evtCounter = readEventCounter();
  rejEvtCounterWllim = readRejectedEventCounterWllim();
  rejEvtCounterWulim = readRejectedEventCounterWulim();
}


// this function configs fpga to read XPOL in full frame mode AUTOTRIGGERING
void pXpolFpga::configFullFrame()
{
  *xpollog::kInfo << "Configuring FPGA in Full Frame mode..." << endline;
  unsigned short conf = (unsigned short)FULLFRAME_MH;
  serialWrite(STATUS_REG,0x1);//reset ADC
  serialWrite(STATUS_REG,0x0);//reset ADC

  //unsigned short disTriggerWidth = 5;//configuration->trgEnableDelay();
  //unsigned short win_dlim = 5;//configuration->maxWindowSize();
  //serialWrite((unsigned short)XPOL_DANGEROUS_REG,
  //	      (disTriggerWidth&0xf) | ((win_dlim<<4) & 0xf0));

  serialWrite((unsigned short)X2NPIXEL_LSB_REG,(char)(PIXELS_TO_READ)&0xff);
  serialWrite((unsigned short)X2NPIXEL_MSB_REG,(char)((PIXELS_TO_READ)>>8)&0xff);
  serialWrite((unsigned short)XPOL_SIGNAL_REG,0);//Disable EnabletriggWindow
  serialWrite((unsigned short)XPOL_DISPIX_REG,0x2);//aaresetn
  serialWrite((unsigned short)XPOL_DISPIX_REG,0x0);//aaresetn
  mainSerialWrite((unsigned short) XPM_TRG_CNT_STATUS_REG,
		  (unsigned short)(XPM_TRG_CNT_STATUS_DATA|0x1));

  //serialWrite((unsigned short)XPOL_WWIDTH_REG,8); What is this?

  serialWrite((unsigned short)XPOL_WPULSE_REG,(conf>>6)&0x7);//modesel,usemh,runb
  serialWrite((unsigned short)XPOL_SIGNAL_REG,(conf>>9)&0x1);//EnabletriggWindow
  //xpol_write_conf(conf&0x3f); /*0x14*/
  
  serialWrite((unsigned short)15,0x0);  //to send configuration to XPOL SIMODE must be at 0
  serialWrite((unsigned short)TOXPOL_MSB_REG,((conf&0x3f)>>4)&0x03);
  serialWrite((unsigned short)TOXPOL_MID_REG,((conf&0x3f)<<4)&0xf0);
  serialWrite((unsigned short)XPOL_SI_CNT_REG,reset_XPOLSI);
  serialWrite((unsigned short)XPOL_SI_CNT_REG,0);
  serialWrite((unsigned short)XPOL_SI_CNT_REG,WSEL);//WSEL=1 CONFIGURATION mode
  serialWrite((unsigned short)XPOL_SI_CNT_REG,SEND|WSEL);//WSEL=1 CONFIGURATION mode
  serialWrite((unsigned short)XPOL_SI_CNT_REG,0);
  unsigned int clockShift = 23;
  unsigned int clockFrequency = 0x40; // 2.5 MHz
  serialWrite((unsigned short)12, clockFrequency | clockShift);
  
  //SET number of readings stored in SRAM
  // Number of events you want to take within a loop
  // 2 because you cannot put more than 2 events in memory  
  serialWrite((unsigned short)XPOL_RDNGS_N_REG,0x2);
  
  setProbes();
}


// this function configs fpga to read XPOL in Windowed mode
void pXpolFpga::configWindowedMode(pDetectorConfiguration *configuration)
{
  *xpollog::kInfo << "Configuring FPGA in Windowed mode..." << endline;	
  unsigned short conf = 0 ;
  if(configuration->readoutMode()==xpoldetector::kChargeInjectionReadoutCode)
    conf = (unsigned short)WINDOWED_INJ;
  else if(configuration->readoutMode()==xpoldetector::kWindowedReadoutCode)
    conf = (unsigned short)WINDOWED_EVT;

  
  // Discharge Width in unit of 50us tics - standard is 10 = 500us
  //serialWrite((unsigned short)14,0xa); 
  unsigned short dischargeWidth = configuration->pedSampleDelay();  
  serialWrite((unsigned short)14, dischargeWidth);
  
  // Trigger Enable Delay after analog reset
  // window dimension limit in unit of 512 channels
  unsigned short disTriggerWidth = configuration->trgEnableDelay();
  unsigned short win_dlim = configuration->maxWindowSize();
  // Put parhentesis here around |.
  serialWrite((unsigned short)XPOL_DANGEROUS_REG,
	      (disTriggerWidth&0xf) | ((win_dlim<<4) & 0xf0));

  serialWrite((unsigned short)STATUS_REG,0);//put at 0 conf[4..0] : needed ?

  serialWrite((unsigned short)XPOL_SIGNAL_REG,0);//Disable EnabletriggWindow  
  serialWrite((unsigned short)XPOL_DISPIX_REG,0x2);//aaresetn

  // XPM_TRG_CNT_STATUS_DATA declared in pUsbController.h
  mainSerialWrite((unsigned short) XPM_TRG_CNT_STATUS_REG, (unsigned short)(XPM_TRG_CNT_STATUS_DATA|0x1));
  
  // Define the Minimum number of pixels to be read in the Window : (DIM = x*32) 
  unsigned short winMinSize = configuration->minWindowSize();
  serialWrite((unsigned short)XPOL_MIN_WIN_DIM_REG, winMinSize);    
  serialWrite((unsigned short)XPOL_WPULSE_REG,(conf>>6)&0x7);//modesel,usemh,runb 
  serialWrite((unsigned short)XPOL_SIGNAL_REG,(conf>>9)&0x1);//EnabletriggWindow

  // Configuration of the sequencer : configuration of the ASIC via the FPGPA
  // 0x0 means : USB speaks with the ASIC via the FPGA
  // 0xf means : the ASIC works in windowed mode				
  serialWrite((unsigned short)15,0x0); //to send configuration to XPOL SIMODE must be at 0
  serialWrite((unsigned short)TOXPOL_MSB_REG,((conf&0x3f)>>4)&0x03); //ASIC configuration

  // In order to set the large margin for the window we should set the LSB
  // of this group of 4 to 1---instead of 0.
  //serialWrite(TOXPOL_MID_REG, ((((conf & 0x3f) | (configuration->windowMarginHigh() & 0x1)) << 4) & 0xf0));
  serialWrite((unsigned short)TOXPOL_MID_REG,((conf&0x3f)<<4)&0xf0);

  //FPGA Sends the content of the register to the ASIC
  serialWrite((unsigned short)XPOL_SI_CNT_REG,reset_XPOLSI);  
  serialWrite((unsigned short)XPOL_SI_CNT_REG,0);
  serialWrite((unsigned short)XPOL_SI_CNT_REG,WSEL);//WSEL=1 CONFIGURATION mode
  serialWrite((unsigned short)XPOL_SI_CNT_REG,SEND|WSEL);//WSEL=1 CONFIGURATION mode
  serialWrite((unsigned short)XPOL_SI_CNT_REG,0);
  
  //SET TIMING CODE 37 FOR 5MHz : Can be set from the GUI, kind of Tack delay
  // MSB correspond to the clock frequency : (Code|Value)&0xF (0x0 is 10MHz, 0x20 is 5MHz, 0x40 is 2.5MHz, 0x60 is 1.25MHz)
  // LSB correspond to the clock shift : Code&0x1F in unit of 25ns
  // 0x37 was standard value
  serialWrite((unsigned short)12, configuration->timingCode());
  
  //SET number of total readings(data+peds)
  // how many pedestals do you want to take between 2 events.
  // When 1 : no pedestal substraction
  unsigned short numSamples = configuration->numPedSamples();
  // We have to write the total number of samples to be acquired, i.e. 1 (the
  // actual data readout) + numSamples (the number of samples for pedestal
  // subtraction).
  serialWrite((unsigned short)XPOL_RDNGS_N_REG, numSamples + 1);

  // When the second bit of this register is set to 1 : analogical reset of the ASIC
  serialWrite((unsigned short)XPOL_DISPIX_REG,0x0);  //aaresetn

  *xpollog::kInfo << "Reading back XPOL registers..." << endline; 
  unsigned short x, y, value;
  readXpolAddressConfigurationRegisters(x, y, value);
  *xpollog::kInfo << "Address x: " << x << endline;
  *xpollog::kInfo << "Address y: " << y << endline;
  *xpollog::kInfo << "Configuration register: 0x" << hex << value << dec
		  << endline;

  // Configuration of the sequencer : configuration of the ASIC via the FPGPA
  // 0x0 means : USB speaks with the ASIC via the FPGA
  // 0xf means : the ASIC works in windowed mode
  serialWrite((unsigned short)15,0xf);
  setProbes();
}


void pXpolFpga::setProbes()
{	
  unsigned short  spare3_probe = 21;
  unsigned short  spare2_probe = 20;
  unsigned short  event_probe  = 2;
  mainSerialWrite((unsigned short) XPM_PROBESPARE3_REG, spare3_probe);
  mainSerialWrite((unsigned short) XPM_PROBESPARE2_REG, spare2_probe);
  mainSerialWrite((unsigned short) XPM_PROBEEVENT_REG,  event_probe);
  *xpollog::kInfo << "Setting probes...  " << spare3_probe << " " << spare2_probe
		  << " "<< event_probe << "..." << endline;
}

void pXpolFpga::configXPM()
{
  *xpollog::kInfo << "Configuring XPM register (FullFrame)." << endline;
  unsigned short adc_pattern=NORMAL;
  // Does not mean anything in Full Frame mode
  // SMALL_BUFFER means sending ~10 events in a loop
  // ~SMALL_BUFFER means sending ~600 events in a loop  
  //mainSerialWrite((unsigned short) XPM_MISC_SEL_REG,(unsigned short) SMALL_BUFFER);
  
  mainSerialWrite((unsigned short) XPM_TIADSCFG_REG,(unsigned short)(ADS_SI_RST|adc_pattern));
  mainSerialWrite((unsigned short) XPM_TIADSCFG_REG,(unsigned short)(ADS_SI_SND|adc_pattern));
  
  mainSerialWrite((unsigned short) XPM_STATUS_REG,(unsigned short) XPOLAUTRIGGH);
  mainSerialWrite((unsigned short) XPM_MEM_MNGR_REG,(unsigned short) DOUBLEBUFF_FFRAME);
  
  // When Writing 2 in 0xA : ADC test
  // When Writing 0 in 0xA : normal operation
  //mainSerialWrite(0xA,2);
  mainSerialWrite(0xA,0);
}


//Duplicated for debugging of Windowed mode
void pXpolFpga::configXPMWindowed(pDetectorConfiguration *configuration)
{
  unsigned short adc_pattern=NORMAL;
  *xpollog::kInfo << "Configuring XPM register (Windowed)." << endline;
  // Buffer Mode (SMALL_BUFFER)
  // 1 means sending ~10 events in a loop
  // 0 means sending ~600 events in a loop
  unsigned short bufferMode = configuration->bufferMode();
  mainSerialWrite((unsigned short) XPM_MISC_SEL_REG, bufferMode);
  *xpollog::kDebug << "Setting buffer mode to " << bufferMode << endline;

  //Now write the settings
  mainSerialWrite((unsigned short) XPM_TIADSCFG_REG,(unsigned short)(ADS_SI_RST|adc_pattern));
  mainSerialWrite((unsigned short) XPM_TIADSCFG_REG,(unsigned short)(ADS_SI_SND|adc_pattern));

  // Write zero in the register : works on the transition 
  mainSerialWrite((unsigned short) XPM_TIADSCFG_REG,0x0);

  //different with default configXPM
  mainSerialWrite((unsigned short) XPM_STATUS_REG, 0x0);
  mainSerialWrite((unsigned short) XPM_MEM_MNGR_REG,(unsigned short) WINDOWED);

  //Tells the FPGA where to inject charge
  // It has to be done here b/c this
  writeXpolAddressRegister(configuration->pixelAddressX(),
			   configuration->pixelAddressY());
  
  *xpollog::kDebug << "Pixel <" << configuration->pixelAddressX() <<
    ", " << configuration->pixelAddressY() <<
    "> selected for charge injection." << endline;
}


//SEND A CONFIGURATION BUFFER TO BOTH DACS
void pXpolFpga::setDacThreshold(pDetectorConfiguration *configuration)
{
  unsigned short us_temp=0, us_tempbuffer[16], copythreshold[15];
  
  // Get the thresholds and the calibration signal values from the configuration
  unsigned short *trh_buffer       = configuration->thresholdDac();
  unsigned short calibrationSignal = configuration->calibrationDac();
  
  // Reads the reference voltage in DAC and sum it to the thresholds - To Be Fixed later
  unsigned short dac = readVrefDac();
  for(int i=0;i<15;i++)
      copythreshold[i] = trh_buffer[i]+ (unsigned short)(dac*AD2_LSB/DAC_LSB);

  us_tempbuffer[0]  = 0x0000|copythreshold[0];
  us_tempbuffer[1]  = 0x1000|copythreshold[1];
  us_tempbuffer[2]  = 0x2000|copythreshold[2];
  us_tempbuffer[3]  = 0x3000|copythreshold[3];
  us_tempbuffer[4]  = 0x4000|copythreshold[4];
  us_tempbuffer[5]  = 0x5000|copythreshold[5];
  us_tempbuffer[6]  = 0x6000|copythreshold[6];
  us_tempbuffer[7]  = 0x7000|copythreshold[7];
  us_tempbuffer[8]  = 0x0000|copythreshold[8];
  us_tempbuffer[9]  = 0x1000|copythreshold[9];
  us_tempbuffer[10] = 0x2000|copythreshold[10];
  us_tempbuffer[11] = 0x3000|copythreshold[11];
  us_tempbuffer[12] = 0x4000|copythreshold[12];
  us_tempbuffer[13] = 0x5000|copythreshold[13];
  us_tempbuffer[14] = 0x6000|copythreshold[14];
  us_tempbuffer[15] = 0x7000|calibrationSignal;  //FOR VTEST on DAC2 : Defines the Charge Injection
   	   

  for(int i=0;i<8;i++)
   {
    us_temp=(TODAC1|SENDDAC);
    mainSerialWrite((unsigned short) XPM_DACWORD_REG,us_tempbuffer[i]);
    mainSerialWrite((unsigned short) XPM_STATUS_REG,us_temp);
    us_temp=((TODAC1) & RSTDACSI);
    mainSerialWrite((unsigned short) XPM_STATUS_REG,us_temp);
   }

  for(int i=8;i<16;i++)
   {
     us_temp=(TODAC2|SENDDAC);
     mainSerialWrite((unsigned short) XPM_DACWORD_REG,us_tempbuffer[i]);
     mainSerialWrite((unsigned short) XPM_STATUS_REG,us_temp);
     us_temp=((TODAC2) & RSTDACSI);
     mainSerialWrite((unsigned short) XPM_STATUS_REG,us_temp);
   }

  serialWrite((unsigned short)XPOL_DISPIX_REG,0X2);//aaresetn
  serialWrite((unsigned short)XPOL_DISPIX_REG,0X0);//aaresetn
  *xpollog::kDebug << "Cluster 0 DAC threshold set to " << us_tempbuffer[0] <<
    endline;
  *xpollog::kDebug << "Calibration signal set to " << calibrationSignal <<
    endline;
}


//writes two configuration words to both DACs
void pXpolFpga::setDacConfig()
{
  unsigned short temp_usbuffer[5],us_temp;
  temp_usbuffer[0]=0xe000;//resetdac registers
  temp_usbuffer[1]=0xf000;//reset controls
  temp_usbuffer[2]=0x8003;//
  temp_usbuffer[3]=0xA003;//ldac
  temp_usbuffer[4]=0xC000;//Powerdown
  for(int i=0;i<5;i++){
    us_temp=(TODAC1|TODAC2|SENDDAC);
    mainSerialWrite((unsigned short) XPM_DACWORD_REG,temp_usbuffer[i]);
    mainSerialWrite((unsigned short) XPM_STATUS_REG,us_temp);
    us_temp=((TODAC1|TODAC2) & RSTDACSI);
    mainSerialWrite((unsigned short) XPM_STATUS_REG,us_temp);
  }
}


unsigned short pXpolFpga::readVrefDac()
{
  // Ask the ADC to start the conversion
  mainSerialWrite((unsigned short)XPM_STATUS_REG, (unsigned short)AD2_RUN);
  // Read the reference voltage in ADC counts.
  unsigned short dac = mainSerialRead((unsigned short)XPM_AD2_DATA);
  //reset AD2_RUN
  mainSerialWrite((unsigned short)XPM_STATUS_REG, 0);
  // Convert in physical units (V).
  double vref = AD2_LSB*dac;
  // Print out the reading.
  *xpollog::kDebug << "Reading reference voltage... " << dac
		   << " ADC counts (" << vref << " V)" << endline;
  // Emit a signal with the relevant value(s).
  emit vrefRead(dac, vref);
  return dac;
}


void pXpolFpga::configDAC(pDetectorConfiguration *configuration) 
{	
  // Ask the ADC to start the conversion
  mainSerialWrite((unsigned short)XPM_STATUS_REG, (unsigned short)AD2_RUN);
  // Configure DAC registers
  setDacConfig();
  // Set DAC thresholds
  setDacThreshold(configuration);
  // Stop the ADC conversion
  mainSerialWrite((unsigned short)XPM_STATUS_REG, 0);
}


void pXpolFpga::setup(pDetectorConfiguration *configuration) 
{
  // Setting FPGA configuration
  if (configuration->readoutMode()==xpoldetector::kChargeInjectionReadoutCode) {
    // Sequence : XPM to be Configured before Windowed Mode
    *xpollog::kInfo << "Starting Windowed Charge Injection Configuration" <<
      endline;
    
    // Configuring DAC settings
    configDAC(configuration);
    *xpollog::kInfo << "DAC Configuration Done." << endline;     
    
    // Configuring XPM register
    // SMALL_BUFFER is set at the beginning of configXPWindowed()
    configXPMWindowed(configuration);     
    *xpollog::kInfo << "XPM Register Configuration Done." << endline;
    
    // Configuring FPGA in windowed mode : WINDOWED_INJ, WINDOWED_EVT
    configWindowedMode(configuration);     
    *xpollog::kInfo << "Windowed charge injection configuration done."
		    << endline;
  } else if (configuration->readoutMode()==xpoldetector::kWindowedReadoutCode){
    // Sequence : XPM to be Configured before Windowed Mode	 
    *xpollog::kInfo << "Starting Windowed Event Configuration" << endline;
    // Configuring DAC settings
    configDAC(configuration);
    //  Configuring XPM register
    configXPMWindowed(configuration);
    // Configuring FPGA in windowed mode : WINDOWED_INJ, WINDOWED_EVT
    configWindowedMode(configuration);      
  } else if(configuration->readoutMode()==xpoldetector::kFullFrameReadoutCode){
    *xpollog::kInfo << "Starting Full Frame Configuration" << endline;
    // Configuring FPGA in full frame mode
    configFullFrame();
    configXPM();
  } else {
    *xpollog::kError << "Unknown Configuration" << endline;
    exit(1);
  }
}
