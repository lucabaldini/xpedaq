#! /bin/env sh

# Static settings.
EXECUTABLE=$XPEDAQ_ROOT/bin/xpedaq
MAX_SECONDS=60
CHARGE_DAC=12
ADDRESS_X=100
ADDRESS_Y=100
NUM_PEDESTALS=0
CLOCK_FREQUENCY=64
CLOCK_SHIFT=22

# Scan settings.
THRESHOLD_DAC=(250)
#(250 260 270 280 290 300 310 320 330 340 350)


echo "Starting timing scan..."
for thr_dac in ${THRESHOLD_DAC[@]}
do
    CMD="$EXECUTABLE -b -I -C $CHARGE_DAC -x $ADDRESS_X -y $ADDRESS_Y -p $NUM_PEDESTALS -f $CLOCK_FREQUENCY -c $CLOCK_SHIFT -s $MAX_SECONDS -T $thr_dac"
    echo "About to execute "$CMD
    $CMD
done
echo "Scan finished, bye!"
