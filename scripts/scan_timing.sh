#! /bin/env sh

# Static settings.
EXECUTABLE=$XPEDAQ_ROOT/bin/xpedaq
NUM_EVENTS=1000
CHARGE_DAC=400
ADDRESS_X=96
ADDRESS_Y=66
NUM_PEDESTALS=4

# Scan settings.
CLOCK_FREQUENCY=(96 64 32)
CLOCK_SHIFT=(16 17 18 19 20 21 22 23 24 25 26 27 28 29 30)


echo "Starting timing scan..."
for clk_freq in ${CLOCK_FREQUENCY[@]}
do
    for clk_shift in ${CLOCK_SHIFT[@]}
    do
	CMD="$EXECUTABLE -b -I -n $NUM_EVENTS -C $CHARGE_DAC -x $ADDRESS_X -y $ADDRESS_Y -p $NUM_PEDESTALS -f $clk_freq -c $clk_shift"
	echo "About to execute "$CMD
	$CMD
    done
done
echo "Scan finished, bye!"
