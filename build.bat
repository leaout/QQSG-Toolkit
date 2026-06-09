@echo off
chcp 437 >nul
setlocal
set "SLN=QQSG-Toolkit.sln"

echo ===== QQSG-Toolkit Build =====
echo.

:: Try vswhere first (most reliable)
set "VSPATH="
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    for /f "tokens=*" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath') do set "VSPATH=%%i"
)

:: Fallback: common paths
if "%VSPATH%"=="" if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" set "VSPATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
if "%VSPATH%"=="" if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" set "VSPATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional"
if "%VSPATH%"=="" if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" set "VSPATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise"
if "%VSPATH%"=="" if exist "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" set "VSPATH=D:\Program Files\Microsoft Visual Studio\2022\Community"
if "%VSPATH%"=="" if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" set "VSPATH=C:\Program Files\Microsoft Visual Studio\2022\Community"

if "%VSPATH%"=="" (
    echo [VS2022 not found]
    echo.
    echo Please open "Developer Command Prompt for VS2022" from Start Menu,
    echo then run:
    echo   cd /d D:\gitpro\empty\QQSG-Toolkit
    echo   msbuild QQSG-Toolkit.sln /p:Configuration=Release /p:Platform=x86
    echo.
    pause
    exit /b 1
)

echo [OK] VS2022: %VSPATH%
call "%VSPATH%\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul
echo [OK] Environment ready
echo.

echo [BUILD] Release x86...
msbuild "%SLN%" /p:Configuration=Release /p:Platform=x86 /t:Rebuild /v:q

if %errorlevel%==0 (
    echo.
    echo ===== SUCCESS =====
    echo DLL: bin\Release\QQSG-Toolkit.dll
    echo.
    echo Usage: rename to msacm32.drv -^> QQ三国 folder
) else (
    echo.
    echo ===== FAILED =====
    echo.
    echo Common fixes:
    echo 1. Run this as Administrator
    echo 2. Check VS2022 has "Desktop development with C++" workload
    echo 3. If d3d9.lib not found, run: "C:\Program Files (x86)\Windows Kits\10\VsDevCmd" x86
    pause
)
pause
