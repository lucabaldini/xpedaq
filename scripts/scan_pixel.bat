@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

REM Static settings.
set EXECUTABLE=%XPEDAQ_ROOT%\bin\xpedaq.exe
set NUM_EVENTS=1000

REM Scan settings.
set ADDRESS_X=20 150 280
set ADDRESS_Y=20 176 332
set CHARGE_DAC=(300, 500, 700)

REM Converting lists into arrays, so we can make a loop over pairs
set i=0
for %%x in (%ADDRESS_X%) do set /A i+=1 & set ADDRESS_X[!i!]=%%x
set i=0
for %%y in (%ADDRESS_Y%) do set /A i+=1 & set ADDRESS_Y[!i!]=%%y
set n=%i%

echo "Starting pixel scan..."
for /L %%i in (1,1,%n%) do (
   for %%D in %CHARGE_DAC% do (
    set X=!ADDRESS_X[%%i]!
    set Y=!ADDRESS_Y[%%i]!
    echo !X! !Y! %%D
    set CMD=%EXECUTABLE% -b -I -n %NUM_EVENTS% -C %%D -x !X! -y !Y!
    echo "About to execute:" !CMD!
    START /WAIT !CMD!
    )
  )
echo "Scan finished, bye!"
