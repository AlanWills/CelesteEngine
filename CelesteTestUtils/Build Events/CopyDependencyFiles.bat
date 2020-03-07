set "OutputDir=%1"
set "Configuration=%2"
set "Platform=%3"

rem Debugging
rem echo %OutputDir% > log.txt
rem echo %OutputDir%..\..\..\..\TestCeleste\bin\Win32\Debug\ >> log.txt

cd %OutputDir%

(robocopy ..\..\..\..\TestUtils\bin\%Platform%\%Configuration%\ .\ /IS /IT /XO) &
(robocopy ..\..\..\..\Celeste\bin\%Platform%\%Configuration%\ .\ /E /IS /IT /XO) &
(robocopy ..\..\..\Resources .\Resources /E /IS /IT /XO) & exit 0