@echo off

if "%1" equ "" goto HINT
if "%2" neq "" goto HINT

set configuration=1

if %1 equ debug set configuration=Debug& goto BUILD
if %1 equ release set configuration=Release& goto BUILD

goto HINT


:BUILD
@echo on
MSBuild.exe ../../generated/%1/ThirdParty/ThirdParty.sln /p:Configuration=%configuration% /t:build -clp:ErrorsOnly;WarningsOnly -nologo -m
rem copy generated\%1\bin\*.dll ..\TacticsVictoryU3D\out\%1 /Y
@echo off
goto EXIT

:HINT
echo.
echo Using build.bat:
echo                 build.bat [debug^|release]
echo.
goto EXIT

:EXIT
