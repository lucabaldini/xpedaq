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
