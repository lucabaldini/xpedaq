This folder is for test data to be used to debug the code---refrain from having
the DAQ output redirected here.

Synopsis

*** test_data_correct.mdat

10 good events.

*** test_data_error.mdat

3 events, with a readout error.
Event 1: good.
Event 2: header is ok, and claiming 1164 bytes, while in fact the header of the
      following event is at 1060 + 13 = 1073 bytes.
Event 3: good, starting at 1060 + 13 bytes and 1140 bytes long.


*** test_fe_500evts.mdat

500 events acquired with the Fe 55 source (5.9 keV). Log file from the DAQ
"""
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
XPOL_DAQ_V2 Run settings file
Created on Wed Apr 27 14:44:58 2016

    clkshift-> 25                        XPOL Freq.->1.25[MHz]
    samples--> 4                        Disc.del-------->10
    Dis. Anrst to Trgw-> 5;                WInd lim-------->5
    sel ch X-> 100                        sel ch Y->100
    loops-> 6000                            referesh->2
Windowed mode (evts)
Monitor net ID:127.0.0.1
Data file: prova_after1dayNoHV_27d14h45m_6k.mdat
Disabled Pixels:

Thresholds:
    Cluster  0:0.28 [V]
    All Thresholds are bounded to Cl0
    Xpol Trg Rev :0.779 [V]
    Vtest        :0.300 [V]
Comments to the run:

Run eneded at on Wed Apr 27 16:58:02 2016
"""


*** test_fe_500evts_th9.root

Processed file, threshold = 9 ADC counts.
