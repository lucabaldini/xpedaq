#!/usr/bin/env python
# Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.
#
# For the license terms see the file LICENSE, distributed along with this
# software.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

import os, sys
import subprocess
import numpy as np

from ixpe_evt_lib import XPOL_NUM_COLUMNS, XPOL_NUM_ROWS, ixpeBinaryFileWindowed


#---------------------------------------
# Useful functions
def guess_trg_pxl(event):
    """ Function to eval pixels that likely triggered the event.
    Finding the triggering minicluster and 
    selecting its pixel with highest pulse height 
    """
    if event.xmax-event.xmin > 17 or event.ymax-event.ymin > 21:
        # window is too large and triggering pixels is too ambiguous
        return (-1, -1)
    # triggering minicluster ids
    if event.xmin>0:
        x0 = 8
    else:
        x0 = event.xmax-9
    if event.ymin>0:
        y0 = 10
    else:
        y0 = event.ymax-11
    submatrix = event.adc_values[x0:x0+2, y0:y0+2]
    x, y = np.unravel_index(np.argmax(submatrix),(2,2))
    return (x+x0+event.xmin, y+y0+event.ymin)
    

def highest_occupancy_pxl(pxl_matrix, npxl=10):
    """ Get the list (col, row, occupancy) of the
    npxl pixels with the highest occpancy
    """
    i = 0
    outlist = []
    while i<npxl:
        row_max, col_max = np.unravel_index(pxl_matrix.argmax(),
                                            pxl_matrix.shape)
        outlist.append((col_max, row_max, pxl_matrix[row_max][col_max]))
        pxl_matrix[row_max][col_max] = 0
        if(outlist[-1][2] == 0):
            i = npxl
        else:			
            i+=1
    return outlist
	
#---------------------------------------

# execute run for noisy pixel
# don't forget to check for min window=32 of pixel on the border won't trigger
EXECUTABLE=os.path.join(os.environ['XPEDAQ_ROOT'],'bin','xpedaq')
MAX_SECONDS=60
MAX_BLOCK= 200
THRESHOLD_DAC=273 #285 DAC -> 230 mV, 200 DAC -> 161 mV, 273 DAC -> 220 mV

""" For reference here list of usefull options:
("max-seconds", 's',"Maximum data acquisition time (in s)");
("max-events", 'n',"Maximum number of events to be aquired");
("max-blocks", 'N',"Maximum number of data buffers to be aquired");
("batch", 'b',"Run in batch mode");  
("threshold-dac", 'T',"Threshold DAC setting (the same for all clusters)");
("comment", 'm', "A user comment");
 """
 
#CMD="$EXECUTABLE -b -I -C $CHARGE_DAC -x $ADDRESS_X -y $ADDRESS_Y -p $NUM_PEDESTALS -f $CLOCK_FREQUENCY -c $CLOCK_SHIFT -s $MAX_SECONDS -T $thr_dac"
cmd = "%s -b -s %d -N %d -T %d" %(EXECUTABLE, MAX_SECONDS, MAX_BLOCK, THRESHOLD_DAC)
print ("\n\tExecuting:\n%s\n..." %cmd)

out_dir_path = ""
run_id = 0
if os.name == "posix": # temporary fix for pcxipe1 with linux where subprocess don't work as expected
	os.system(cmd)
	try:
		run_id = input("enter run id ")
	except NameError:
		run_id = raw_input("enter run id ")	
	out_dir_path = "/data/xpedata/004/" + run_id
else:
	proc=subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	for line in proc.stdout:
		sys.stdout.buffer.write(line)
		sys.stdout.flush()
		if "Creating" in str(line):
			out_dir_path = str(line).split("...")[0].split("Creating ")[-1]
			run_id = os.path.basename(out_dir_path)
	proc.wait()

# execute search for noisy pixels 
print ("\n\tAnalyzing run %s in %s" % (run_id, out_dir_path))

# parsing run stats
run_stat_file = open(os.path.join(out_dir_path, '%s_runstat.txt'% (run_id)))
run_n_evt = 0
run_duration = 0 #s
print("Run stats for run %s" % run_id)
for l in run_stat_file:
	print(l.strip('\n'))
	if 'duration' in l:
		run_duration = float(l.strip('\n').split(':')[-1])
	if 'events' in l:
		run_n_evt = int(l.strip('\n').split(':')[-1])
print("Average rate [Hz]: %f" % (run_n_evt/run_duration))

file_path = os.path.join(out_dir_path, '%s_data.mdat'% (run_id))
input_file = ixpeBinaryFileWindowed(file_path)
# matrix for pixel occupancy
# DON"T FORGET: np.zeros([row,col])
pxl_occupancy = np.zeros([XPOL_NUM_ROWS,XPOL_NUM_COLUMNS])

for i in range(run_n_evt):
    event = input_file.next()
    #event.draw_ascii(0)
    col, row = guess_trg_pxl(event)
    pxl_occupancy[row][col] += 1
    #print ("debug col, row: %d %d" % (col, row))
    #input("e2g")

run_noisy_pxl =  highest_occupancy_pxl(pxl_occupancy, 100)
print ("List of noisy pixel\ncol\trow\tocc.\trate")
nTot = 0
for  (c,r,n) in run_noisy_pxl:
	print ("%d\t%d\t%d\t%.2f" % (c,r,n,n/run_duration))
	nTot +=n
print ("Tot triggers %d, total rate %.2f" %(nTot, nTot/run_duration))

# Write trigger mask
print ("\n\tWriting trigger mask \n NOT IMPLEMENTED YET" )

#