# ORORA AI - Derleme Scripti (Hızlı)
# Bağımlılıklar zaten kuruluysa kullanın

param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release",
    [switch]$Clean,
    [switch]$Run,
    [switch]$Test
)

$ErrorActionPreference = "Stop"
$ProjectRoot = $PSScriptRoot
$BuildDir = Join-Path $ProjectRoot "build"

Write-Host "ORORA AI - Derleme ($Configuration)" -ForegroundColor Cyan

# Temizlik
if ($Clean -and (Test-Path $BuildDir)) {
    Write-Host "Temizleniyor..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $BuildDir
}

# CMake configure
if (-not (Test-Path "$BuildDir\CMakeCache.txt")) {
    Write-Host "CMake yapılandırılıyor..." -ForegroundColor Yellow
    
    $toolchain = ""
    if ($env:VCPKG_ROOT) {
        $toolchain = "-DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
    }
    
    cmake -B $BuildDir -S $ProjectRoot `
        -DCMAKE_BUILD_TYPE=$Configuration `
        $toolchain
}

# Build
Write-Host "Derleniyor..." -ForegroundColor Yellow
cmake --build $BuildDir --config $Configuration --parallel

if ($LASTEXITCODE -ne 0) {
    Write-Host "Derleme başarısız!" -ForegroundColor Red
    exit 1
}

Write-Host "Derleme tamamlandı!" -ForegroundColor Green

# Test
if ($Test) {
    Write-Host "Testler çalıştırılıyor..." -ForegroundColor Yellow
    Push-Location $BuildDir
    ctest --output-on-failure -C $Configuration
    Pop-Location
}

# Çalıştır
if ($Run) {
    $exe = Join-Path $BuildDir "bin\$Configuration\orora.exe"
    if (-not (Test-Path $exe)) {
        $exe = Join-Path $BuildDir "bin\orora.exe"
    }
    
    if (Test-Path $exe) {
        Write-Host "Çalıştırılıyor: $exe" -ForegroundColor Cyan
        & $exe
    }
    else {
        Write-Host "Executable bulunamadı: $exe" -ForegroundColor Red
    }
}
