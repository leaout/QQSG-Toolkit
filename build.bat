@echo off
cd /d "%~dp0"
setlocal

echo ===== QQSG-Toolkit Build =====
echo.

:: Find VS2022
set "VSPATH="
for %%P in (
  "C:\Program Files\Microsoft Visual Studio\2022\Community"
  "C:\Program Files\Microsoft Visual Studio\2022\Professional"
  "C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
  "D:\Program Files\Microsoft Visual Studio\2022\Community"
) do if exist "%%~P\VC\Auxiliary\Build\vcvarsall.bat" set "VSPATH=%%~P"

if "%VSPATH%"=="" (
  echo [FAIL] VS2022 with C++ workload not found.
  echo Install it from: https://visualstudio.microsoft.com/
  pause & exit /b 1
)

echo [OK] VS2022: %VSPATH%

:: Compiler environment
call "%VSPATH%\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul
echo [OK] Compiler ready

:: Set up VCTargetsPath
set "VCTargetsPath=%VSPATH%\MSBuild\Microsoft\VC\v170\"
set "VCToolsPath=%VSPATH%\MSBuild\Microsoft\VC\v170\"

echo.
echo [BUILD] Release Win32...
msbuild "%~dp0QQSG-Toolkit.sln" /p:Configuration=Release /p:Platform=Win32 /t:Rebuild /v:q

if %errorlevel%==0 (
  echo.
  echo ===== SUCCESS =====
  echo DLL: bin\Win32\Release\QQSG-Toolkit.dll
  echo.
  echo Usage: rename to msacm32.drv, copy to QQ game folder
) else (
  echo.
  echo ===== FAILED =====
  echo Try opening QQSG-Toolkit.sln in VS2022 and build from IDE.
  pause
)
pause
