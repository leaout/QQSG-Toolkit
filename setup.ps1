# QQSG-Toolkit 编译环境配置脚本
# 以管理员身份在项目根目录运行

Write-Host "=== QQSG-Toolkit 编译环境配置 ===" -ForegroundColor Cyan
Write-Host ""

# 检查 VS2022 是否安装
$vsPath = & "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" 2>$null
if (-not $vsPath) {
    $vsPath = & "${env:ProgramFiles}\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" 2>$null
}
if (-not $vsPath) {
    $vsPath = & "${env:ProgramFiles}\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe" 2>$null
}

if ($vsPath) {
    Write-Host "[OK] VS2022 已安装" -ForegroundColor Green
} else {
    Write-Host "[!] VS2022 未检测到，请确保已安装" -ForegroundColor Yellow
}

# 检查环境
Write-Host ""
Write-Host "步骤1: 配置 D3DX9" -ForegroundColor Cyan
Write-Host "-------------------------"

$d3dx9Include = Join-Path $PSScriptRoot "extern\d3dx9\Include"
$d3dx9Lib = Join-Path $PSScriptRoot "extern\d3dx9\Lib"

if (Test-Path (Join-Path $d3dx9Include "d3dx9.h")) {
    Write-Host "[OK] d3dx9.h 已找到" -ForegroundColor Green
} else {
    Write-Host "[!] 需要安装 DirectX SDK (June 2010)" -ForegroundColor Yellow
    Write-Host "    下载地址: https://www.microsoft.com/en-us/download/details.aspx?id=6812" -ForegroundColor White
    Write-Host "    安装后 $(DXSDK_DIR) 环境变量会自动设置，项目会自动引用。" -ForegroundColor White
    Write-Host ""
    Write-Host "    或者使用 NuGet 方式安装 Microsoft.DXSDK.D3DX:" -ForegroundColor White
    Write-Host "    dotnet add package Microsoft.DXSDK.D3DX" -ForegroundColor White
}

# 检查 Detours
Write-Host ""
Write-Host "步骤2: 检查 Detours" -ForegroundColor Cyan
Write-Host "----------------------"

$detoursLib = Join-Path $PSScriptRoot "extern\detours\lib.X86\detours.lib"
if (Test-Path $detoursLib) {
    Write-Host "[OK] detours.lib 已就绪" -ForegroundColor Green
} else {
    Write-Host "[!] detours.lib 未找到" -ForegroundColor Red
    Write-Host "    请从 Microsoft Detours 仓库下载: https://github.com/microsoft/Detours" -ForegroundColor White
}

# Windows SDK 检查
Write-Host ""
Write-Host "步骤3: 检查 Windows SDK" -ForegroundColor Cyan
Write-Host "-------------------------"

$windowsKits = "${env:ProgramFiles(x86)}\Windows Kits\10\Include"
if (Test-Path $windowsKits) {
    Write-Host "[OK] Windows SDK 已安装" -ForegroundColor Green
} else {
    Write-Host "[!] Windows SDK 未找到，请随 VS2022 一起安装" -ForegroundColor Yellow
}

# 编译测试
Write-Host ""
Write-Host "步骤4: 编译" -ForegroundColor Cyan
Write-Host "-------------"
Write-Host "打开 QQSG-Toolkit.sln，选择:" -ForegroundColor White
Write-Host "  配置: Release" -ForegroundColor White
Write-Host "  平台: x86" -ForegroundColor White
Write-Host ""
Write-Host "然后点击 生成 -> 生成解决方案" -ForegroundColor White
Write-Host ""
Write-Host "生成的 DLL 在: bin\Release\QQSG-Toolkit.dll" -ForegroundColor Green
Write-Host "使用时重命名为 msacm32.drv 放入三国根目录" -ForegroundColor Yellow
Write-Host ""
Write-Host "=== 配置完成 ===" -ForegroundColor Cyan

pause
