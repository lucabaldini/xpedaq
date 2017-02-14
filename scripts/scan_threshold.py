import os, sys
import subprocess

EXECUTABLE=os.path.join(os.environ['XPEDAQ_ROOT'],'bin','xpedaq')
MAX_SECONDS=5
#MAX_BLOCK= 20
THRESHOLD_DAC=285 # 230 mV

#CMD="$EXECUTABLE -b -I -C $CHARGE_DAC -x $ADDRESS_X -y $ADDRESS_Y -p $NUM_PEDESTALS -f $CLOCK_FREQUENCY -c $CLOCK_SHIFT -s $MAX_SECONDS -T $thr_dac"
cmd = "%s -b -s %d -T %d" %(EXECUTABLE, MAX_SECONDS, THRESHOLD_DAC)
print ("\n\tExecuting:\n%s\n..." %cmd)

out_dir_path = ""
run_id = 0
proc=subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
for line in proc.stdout:
	sys.stdout.buffer.write(line)
	sys.stdout.flush()
	if "Creating" in str(line):
		out_dir_path = str(line).split("...")[0].split("Creating ")[-1]
		run_id = os.path.basename(out_dir_path)
proc.wait()

print ("\n\tReady to analyze run %s in %s" % (run_id, out_dir_path))


""" For reference here list of usefull options:
("max-seconds", 's',"Maximum data acquisition time (in s)");
("max-events", 'n',"Maximum number of events to be aquired");
("max-blocks", 'N',"Maximum number of data buffers to be aquired");
("batch", 'b',"Run in batch mode");  
("threshold-dac", 'T',"Threshold DAC setting (the same for all clusters)");
("comment", 'm', "A user comment");
 """