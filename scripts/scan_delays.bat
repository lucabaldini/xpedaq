@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

REM Static settings.
SET EXECUTABLE=%XPEDAQ_ROOT%\bin\xpedaq.exe
SET NUM_EVENTS=100
SET NUM_PEDESTALS=1

REM Scan settings.
SET SAMPLE_DELAY=(10 8)
SET TRG_DELAY=(5 3)


echo "Starting timing scan..."
FOR %%S IN %SAMPLE_DELAY% DO (
	FOR %%T in %TRG_DELAY% DO (
		echo %%S %%T
		SET CMD=%EXECUTABLE% -b -n %NUM_EVENTS% -p %NUM_PEDESTALS% -d %%S -D %%T
		echo "About to execute "!CMD!
		START /WAIT !CMD!
		)
	)

echo "Scan finished, bye!"
