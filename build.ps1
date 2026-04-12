# Spirit Space — Build & Run Script
# Run from the spirit_space/ directory:
#   powershell -ExecutionPolicy Bypass -File build.ps1
#
# Optional flags:
#   -Clean     Delete the build/ directory before configuring
#   -RunAfter  Launch the executable after a successful build
#   -Config    Build configuration: Release (default) or Debug

param(
    [switch]$Clean,
    [switch]$RunAfter,
    [string]$Config = "Release"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# ── Locate vcpkg ──────────────────────────────────────────────────────────────
# Add or change entries in this list to match your vcpkg install location.
$vcpkgCandidates = @(
    "C:\vcpkg\vcpkg.exe",
    "C:\src\vcpkg\vcpkg.exe",
    "$env:USERPROFILE\vcpkg\vcpkg.exe",
    "$env:LOCALAPPDATA\vcpkg\vcpkg.exe",
    "C:\dev\vcpkg\vcpkg.exe",
    "C:\tools\vcpkg\vcpkg.exe"
)

$vcpkgExe = $null
foreach ($candidate in $vcpkgCandidates) {
    if (Test-Path $candidate) {
        $vcpkgExe = $candidate
        break
    }
}

if (-not $vcpkgExe) {
    Write-Host ""
    Write-Host "ERROR: vcpkg not found." -ForegroundColor Red
    Write-Host ""
    Write-Host "Set your vcpkg path by editing build.ps1 and adding your path to `$vcpkgCandidates:"
    Write-Host '    $vcpkgCandidates = @("C:\your\path\to\vcpkg\vcpkg.exe", ...)'
    Write-Host ""
    Write-Host "Or install vcpkg:"
    Write-Host "    git clone https://github.com/microsoft/vcpkg C:\vcpkg"
    Write-Host "    C:\vcpkg\bootstrap-vcpkg.bat -disableMetrics"
    Write-Host ""
    exit 1
}

$vcpkgRoot = Split-Path $vcpkgExe -Parent
$vcpkgToolchain = Join-Path $vcpkgRoot "scripts\buildsystems\vcpkg.cmake"

Write-Host "vcpkg : $vcpkgExe" -ForegroundColor Cyan

# ── Locate the project root (directory containing this script) ─────────────
$projectRoot = $PSScriptRoot
$buildDir    = Join-Path $projectRoot "build"
$exePath     = Join-Path $buildDir "bin\$Config\spirit_space.exe"

Write-Host "Root  : $projectRoot"
Write-Host "Build : $buildDir"
Write-Host "Config: $Config"
Write-Host ""

# ── Optional clean ─────────────────────────────────────────────────────────
if ($Clean -and (Test-Path $buildDir)) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $buildDir
}

# ── Install vcpkg dependencies ─────────────────────────────────────────────
Write-Host "Installing vcpkg dependencies..." -ForegroundColor Cyan
Push-Location $projectRoot
& $vcpkgExe install --triplet x64-windows
if ($LASTEXITCODE -ne 0) {
    Write-Host "vcpkg install failed." -ForegroundColor Red
    Pop-Location
    exit $LASTEXITCODE
}
Pop-Location

# ── Configure ─────────────────────────────────────────────────────────────
Write-Host ""
Write-Host "Configuring CMake..." -ForegroundColor Cyan
cmake -B $buildDir `
      -S $projectRoot `
      -G "Visual Studio 16 2019" `
      -A x64 `
      -DCMAKE_TOOLCHAIN_FILE="$vcpkgToolchain" `
      -DVCPKG_TARGET_TRIPLET=x64-windows

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed." -ForegroundColor Red
    exit $LASTEXITCODE
}

# ── Build ──────────────────────────────────────────────────────────────────
Write-Host ""
Write-Host "Building ($Config)..." -ForegroundColor Cyan
cmake --build $buildDir --config $Config --parallel

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host ""
Write-Host "Build succeeded." -ForegroundColor Green
Write-Host "Executable: $exePath"

# ── Run ────────────────────────────────────────────────────────────────────
if ($RunAfter) {
    if (Test-Path $exePath) {
        Write-Host ""
        Write-Host "Launching..." -ForegroundColor Cyan
        & $exePath
    } else {
        Write-Host "Executable not found at expected path: $exePath" -ForegroundColor Red
        exit 1
    }
}
