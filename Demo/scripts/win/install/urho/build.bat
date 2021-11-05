@echo off

if "%1" equ "" goto HINT
if "%2" neq "" goto HINT

set configuration=1

if %1 equ debug set configuration=Debug& goto BUILD
if %1 equ release set configuration=Release& goto BUILD

goto HINT


:BUILD
@echo on
MSBuild.exe ../../../../generated/U-Urho/%1/Urho3D.sln /p:Configuration=%configuration% /t:build -clp:ErrorsOnly;WarningsOnly -nologo -m
cd ../../../../
copy generated\U-Urho\%1\bin\*.dll out\%1 /Y
cd scripts/win/install/urho
@echo off
goto EXIT

:HINT
echo.
echo Using build.bat:
echo                 build.bat [debug^|release]
echo.
goto EXIT

:EXIT
