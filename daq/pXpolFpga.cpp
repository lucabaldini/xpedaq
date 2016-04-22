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


//read a 16-bit word from fpga serial interface B
//PE[3] is SCLK, PE[7] is SO_EN, PE[6] is datain (output from fpga)
//MSb first!!!!!!
unsigned short pXpolFpga::mainSiRead(unsigned short reg_address){
  *xpollog::kInfo << "CS: begin mainSiRead with reg_address " << reg_address << endline;
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


void pXpolFpga::writeAddress(unsigned short X, unsigned short Y){
  serialWrite(15,0);
  serialWrite(TOXPOL_MSB_REG,(X>>7)&0x03);
  serialWrite(TOXPOL_MID_REG,((X<<1)&0xfe)|((Y>>8)&0x1));
  serialWrite(TOXPOL_LSB_REG,Y&0xFF);
  serialWrite(XPOL_SI_CNT_REG,reset_XPOLSI);
  serialWrite(XPOL_SI_CNT_REG,0);
  serialWrite(XPOL_SI_CNT_REG,SEND);//WSEL=0 address mode
  serialWrite(XPOL_SI_CNT_REG,0);	
}


// this function configs fpga to read XPOL in full frame mode AUTOTRIGGERING
void pXpolFpga::configFullFrame(unsigned short conf)
{
  *xpollog::kInfo << "Configuring FPGA in Full Frame mode..." << endline;
  serialWrite(STATUS_REG,0x1);//reset ADC
  serialWrite(STATUS_REG,0x0);//reset ADC
  serialWrite((unsigned short)X2NPIXEL_LSB_REG,(char)(PIXELS_TO_READ)&0xff);
  serialWrite((unsigned short)X2NPIXEL_MSB_REG,(char)((PIXELS_TO_READ)>>8)&0xff);
  serialWrite((unsigned short)XPOL_SIGNAL_REG,0);//Disable EnabletriggWindow
  serialWrite((unsigned short)XPOL_DISPIX_REG,0X2);//aaresetn
  serialWrite((unsigned short)XPOL_DISPIX_REG,0X0);//aaresetn
  mainSerialWrite(0xa,1);

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
  serialWrite((unsigned short)12,0X37);//SET TIMING CODE 37 FOR 5MHz
  
  //SET number of readings stored in SRAM
  // Number of events you want to take within a loop
  // 2 because you cannot put more than 2 events in memory  
  serialWrite((unsigned short)XPOL_RDNGS_N_REG,0x2);
}

// this function configs fpga to read XPOL in Windowed mode
void pXpolFpga::configWindowedMode(pDetectorConfiguration *configuration)
{
  *xpollog::kInfo << "Configuring FPGA in Windowed mode..." << endline;	
  unsigned short conf = 0 ;
  if(configuration->getReadoutMode()==xpoldetector::kChargeInjectionReadoutCode)
    conf = (unsigned short)WINDOWED_INJ;
  else if(configuration->getReadoutMode()==xpoldetector::kWindowedReadoutCode)
    conf = (unsigned short)WINDOWED_EVT;

  
  // Discharge Width in unit of 50us tics - standard is 10 = 500us
  //serialWrite((unsigned short)14,0xa); 
  unsigned short dischargeWidth = configuration->getPedSampleDelay();  
  serialWrite((unsigned short)14, dischargeWidth);
  
  // Trigger Enable Delay after analog reset
  // window dimension limit in unit of 512 channels
  unsigned short disTriggerWidth = configuration->getTrgEnableDelay();
  unsigned short win_dlim = configuration->getMaxWindowSize();
  // Put parhentesis here around |.
  serialWrite((unsigned short)XPOL_DANGEROUS_REG,
	      (disTriggerWidth&0xf) | ((win_dlim<<4) & 0xf0));

  serialWrite((unsigned short)STATUS_REG,0);//put at 0 conf[4..0] : needed ?

  serialWrite((unsigned short)XPOL_SIGNAL_REG,0);//Disable EnabletriggWindow  
  serialWrite((unsigned short)XPOL_DISPIX_REG,0x2);//aaresetn

  // XPM_TRG_CNT_STATUS_DATA declared in pUsbController.h
  mainSerialWrite((unsigned short) XPM_TRG_CNT_STATUS_REG, (unsigned short)(XPM_TRG_CNT_STATUS_DATA|0x1));
  
  // Define the Minimum number of pixels to be read in the Window : (DIM = x*32) 
  unsigned short winMinSize = configuration->getMinWindowSize();
  serialWrite((unsigned short)XPOL_MIN_WIN_DIM_REG, winMinSize);    
  serialWrite((unsigned short)XPOL_WPULSE_REG,(conf>>6)&0x7);//modesel,usemh,runb 
  serialWrite((unsigned short)XPOL_SIGNAL_REG,(conf>>9)&0x1);//EnabletriggWindow

  // Configuration of the sequencer : configuration of the ASIC via the FPGPA
  // 0x0 means : USB speaks with the ASIC via the FPGA
  // 0xf means : the ASIC works in windowed mode				
  serialWrite((unsigned short)15,0x0); //to send configuration to XPOL SIMODE must be at 0
  serialWrite((unsigned short)TOXPOL_MSB_REG,((conf&0x3f)>>4)&0x03); //ASIC configuration
  serialWrite((unsigned short)TOXPOL_MID_REG,((conf&0x3f)<<4)&0xf0); //ASIC configuration

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
  serialWrite((unsigned short)12, configuration->getTimingCode());
  
  //SET number of total readings(data+peds)
  // how many pedestals do you want to take between 2 events.
  // When 1 : no pedestal substraction
  unsigned short numSamples = configuration->getNumPedSamples();
  // We have to write the total number of samples to be acquired, i.e. 1 (the
  // actual data readout) + numSamples (the number of samples for pedestal
  // subtraction).
  serialWrite((unsigned short)XPOL_RDNGS_N_REG, numSamples + 1);
  // Configuration of the sequencer : configuration of the ASIC via the FPGPA
  // 0x0 means : USB speaks with the ASIC via the FPGA
  // 0xf means : the ASIC works in windowed mode				
  serialWrite((unsigned short)15,0xf);

  // When the second bit of this register is set to 1 : analogical reset of the ASIC
  serialWrite((unsigned short)XPOL_DISPIX_REG,0x0);  //aaresetn      
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
  unsigned short bufferMode = configuration->getBufferMode();
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
  writeAddress(configuration->getPixelAddressX(), configuration->getPixelAddressY());
  
  *xpollog::kDebug << "Pixel (" << configuration->getPixelAddressX() <<
    ", " << configuration->getPixelAddressY() <<
    ") selected for charge injection." << endline;
}


//SEND A CONFIGURATION BUFFER TO BOTH DACS
void pXpolFpga::setDacThreshold(pDetectorConfiguration *configuration)
{
  unsigned short us_temp=0, us_tempbuffer[16], copythreshold[15];
  
  // Get the thresholds and the calibration signal values from the configuration
  unsigned short *trh_buffer       = configuration->getThresholdDac();
  unsigned short calibrationSignal = configuration->getCalibrationDac();
  
  // Reads the reference voltage in DAC and sum it to the thresholds - To Be Fixed later
  unsigned short vref_dac = readVrefDac();
  for(int i=0;i<15;i++)
      copythreshold[i] = trh_buffer[i]+ (unsigned short)(vref_dac*AD2_LSB/DAC_LSB);

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
  // Ask the ADC to start the conversion !!! AIUTO!!???
  mainSerialWrite((unsigned short) XPM_STATUS_REG,(unsigned short) AD2_RUN);
  // Read the register
  unsigned short vref_dac = mainSiRead((unsigned short)XPM_AD2_DATA);
  *xpollog::kInfo << "CS: reading value vref_dac " << vref_dac << endline;	
  // Emit a signal to update the main GUI
  emit thresholdRefRead(vref_dac); 
  return vref_dac;
}

void pXpolFpga::configDAC(pDetectorConfiguration *configuration) 
{	
  // Ask the ADC to start the conversion
  mainSerialWrite((unsigned short) XPM_STATUS_REG,(unsigned short) AD2_RUN);

  // Reads the reference voltage in DAC
  unsigned short vref_dac = readVrefDac();
  // Convert the reference voltage in Volts
  float m_vref = AD2_LSB * vref_dac;
  // Print the reference voltage value in the log
  *xpollog::kInfo << "Reading reference voltage... " << m_vref << endline;
  // Configure DAC registers
  setDacConfig();
  // Set DAC thresholds - 
  setDacThreshold(configuration);
  // Stop the ADC conversion
  mainSerialWrite((unsigned short) XPM_STATUS_REG,(unsigned short) 0);
}

void pXpolFpga::setup(pDetectorConfiguration *configuration) 
{
  // Setting FPGA configuration
  if(configuration->getReadoutMode()==xpoldetector::kChargeInjectionReadoutCode){
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
    *xpollog::kInfo << "Windowed Charge Injection Configuration Done." << endline;
  } else if(configuration->getReadoutMode()==xpoldetector::kWindowedReadoutCode){
    // Sequence : XPM to be Configured before Windowed Mode	 
    *xpollog::kInfo << "Starting Windowed Event Configuration" << endline;
    // Configuring DAC settings
    configDAC(configuration);
    //  Configuring XPM register
    configXPMWindowed(configuration);
    // Configuring FPGA in windowed mode : WINDOWED_INJ, WINDOWED_EVT
    configWindowedMode(configuration);      
  } else if(configuration->getReadoutMode()==xpoldetector::kFullFrameReadoutCode){
    *xpollog::kInfo << "Starting Full Frame Configuration" << endline;
    // Configuring FPGA in full frame mode
    configFullFrame((unsigned short)FULLFRAME_MH);
    configXPM();
  } else {
    *xpollog::kError << "Unknown Configuration" << endline;
    exit(1);
  }
}
