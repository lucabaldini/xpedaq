@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

REM Static settings.
SET EXECUTABLE=%XPEDAQ_ROOT%\bin\xpedaq.exe
SET NUM_EVENTS=10000
SET NUM_PEDESTALS=1

REM Scan settings.
SET SAMPLE_DELAY=(18 16 14 12 10 9 8 7 6 5 4)
SET TRG_DELAY=(8 6 5 4 3 2 1)


echo "Starting timing scan..."
FOR %%T in %TRG_DELAY% DO (
    FOR %%S IN %SAMPLE_DELAY% DO (
		echo %%S %%T
		SET CMD=%EXECUTABLE% -b -n %NUM_EVENTS% -p %NUM_PEDESTALS% -d %%S -D %%T
		echo "About to execute "!CMD!
		START /WAIT !CMD!
		)
	)

echo "Scan finished, bye!"
