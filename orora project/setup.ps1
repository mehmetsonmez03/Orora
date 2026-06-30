# ORORA AI - Otomatik Kurulum Scripti
# PowerShell 7+ gerektirir - Windows 10/11

param(
    [switch]$SkipDependencies,
    [switch]$SkipModels,
    [switch]$Debug,
    [switch]$Help
)

$ErrorActionPreference = "Stop"
$ProjectRoot = $PSScriptRoot

function Write-Banner {
    Write-Host @"
╔══════════════════════════════════════════════════════════════╗
║                    ORORA AI - Kurulum                        ║
║            Bilinçli Yapay Zeka Asistanı v2.0                 ║
╚══════════════════════════════════════════════════════════════╝
"@ -ForegroundColor Cyan
}

function Write-Step { param($step, $msg) Write-Host "[$step] $msg" -ForegroundColor Yellow }
function Write-Success { param($msg) Write-Host "✅ $msg" -ForegroundColor Green }
function Write-Info { param($msg) Write-Host "ℹ️  $msg" -ForegroundColor Cyan }
function Write-Error { param($msg) Write-Host "❌ $msg" -ForegroundColor Red }

function Show-Help {
    Write-Host @"
ORORA AI - Kurulum Scripti
===========================

Kullanım:
  .\setup.ps1                    # Tam kurulum
  .\setup.ps1 -SkipDependencies  # Bağımlılıkları atla
  .\setup.ps1 -SkipModels        # Model indirmesini atla
  .\setup.ps1 -Debug             # Debug build
  .\setup.ps1 -Help              # Bu yardım

Gereksinimler:
  - Windows 10/11 64-bit
  - Visual Studio 2022 (C++ Desktop Development)
  - CMake 3.20+
  - Git
  - Qt6 (isteğe bağlı - vcpkg ile yüklenir)

"@
}

function Check-Prerequisites {
    Write-Step "1/6" "Gereksinimler kontrol ediliyor..."
    
    # CMake
    $cmake = Get-Command cmake -ErrorAction SilentlyContinue
    if (-not $cmake) {
        Write-Error "CMake bulunamadı! https://cmake.org/download/ adresinden indirin."
        return $false
    }
    Write-Info "CMake: $($cmake.Source)"
    
    # Git
    $git = Get-Command git -ErrorAction SilentlyContinue
    if (-not $git) {
        Write-Error "Git bulunamadı! https://git-scm.com/download/win adresinden indirin."
        return $false
    }
    Write-Info "Git: $($git.Source)"
    
    # Visual Studio / MSVC
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $vsPath = & $vswhere -latest -property installationPath
        if ($vsPath) {
            Write-Info "Visual Studio: $vsPath"
        }
    }
    
    Write-Success "Tüm gereksinimler mevcut"
    return $true
}

function Install-Vcpkg {
    Write-Step "2/6" "vcpkg kurulumu..."
    
    $vcpkgPath = Join-Path $ProjectRoot "vcpkg"
    
    if (Test-Path $vcpkgPath) {
        Write-Info "vcpkg zaten mevcut"
        return $vcpkgPath
    }
    
    Write-Info "vcpkg klonlanıyor..."
    git clone https://github.com/microsoft/vcpkg.git $vcpkgPath
    
    Push-Location $vcpkgPath
    try {
        Write-Info "vcpkg bootstrap..."
        .\bootstrap-vcpkg.bat
    }
    finally {
        Pop-Location
    }
    
    # Ortam değişkeni
    $env:VCPKG_ROOT = $vcpkgPath
    Write-Success "vcpkg kuruldu: $vcpkgPath"
    
    return $vcpkgPath
}

function Install-Dependencies {
    param($vcpkgPath)
    
    Write-Step "3/6" "Bağımlılıklar yükleniyor..."
    
    $vcpkg = Join-Path $vcpkgPath "vcpkg.exe"
    
    $packages = @(
        "qt6:x64-windows",
        "assimp:x64-windows",
        "nlohmann-json:x64-windows",
        "glm:x64-windows"
    )
    
    foreach ($pkg in $packages) {
        Write-Info "Yükleniyor: $pkg"
        & $vcpkg install $pkg
    }
    
    # ONNX Runtime (özel kurulum)
    Write-Info "ONNX Runtime kontrolü..."
    $onnxDir = Join-Path $ProjectRoot "external\onnxruntime"
    if (-not (Test-Path $onnxDir)) {
        Write-Info "ONNX Runtime indiriliyor..."
        $onnxUrl = "https://github.com/microsoft/onnxruntime/releases/download/v1.16.3/onnxruntime-win-x64-1.16.3.zip"
        $onnxZip = Join-Path $env:TEMP "onnxruntime.zip"
        
        Invoke-WebRequest -Uri $onnxUrl -OutFile $onnxZip
        Expand-Archive -Path $onnxZip -DestinationPath (Join-Path $ProjectRoot "external") -Force
        Rename-Item -Path (Join-Path $ProjectRoot "external\onnxruntime-win-x64-1.16.3") -NewName "onnxruntime"
        
        Remove-Item $onnxZip
    }
    
    Write-Success "Bağımlılıklar yüklendi"
}

function Download-Models {
    Write-Step "4/6" "AI Modelleri indiriliyor..."
    
    $downloadScript = Join-Path $ProjectRoot "models\download_models.ps1"
    if (Test-Path $downloadScript) {
        & $downloadScript -All
    }
    else {
        Write-Warning "Model indirme scripti bulunamadı"
    }
    
    Write-Success "Modeller hazır"
}

function Build-Project {
    param([bool]$IsDebug)
    
    Write-Step "5/6" "Proje derleniyor..."
    
    $buildDir = Join-Path $ProjectRoot "build"
    $buildType = if ($IsDebug) { "Debug" } else { "Release" }
    
    # CMake configure
    Write-Info "CMake yapılandırılıyor ($buildType)..."
    
    $cmakeArgs = @(
        "-B", $buildDir,
        "-S", $ProjectRoot,
        "-DCMAKE_BUILD_TYPE=$buildType",
        "-DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake",
        "-DORORA_BUILD_TESTS=ON"
    )
    
    cmake @cmakeArgs
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake yapılandırma başarısız!"
        return $false
    }
    
    # CMake build
    Write-Info "Derleniyor..."
    cmake --build $buildDir --config $buildType --parallel
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Derleme başarısız!"
        return $false
    }
    
    Write-Success "Derleme tamamlandı: $buildDir\bin\orora.exe"
    return $true
}

function Run-Tests {
    Write-Step "6/6" "Testler çalıştırılıyor..."
    
    $buildDir = Join-Path $ProjectRoot "build"
    
    Push-Location $buildDir
    try {
        ctest --output-on-failure
    }
    finally {
        Pop-Location
    }
    
    Write-Success "Testler tamamlandı"
}

function Show-Summary {
    Write-Host ""
    Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Green
    Write-Host "                    KURULUM TAMAMLANDI!                         " -ForegroundColor Green
    Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Green
    Write-Host ""
    Write-Host "Çalıştırmak için:" -ForegroundColor White
    Write-Host "  .\build\bin\orora.exe" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Veya Visual Studio ile:" -ForegroundColor White
    Write-Host "  1. orora\ORORA_Core.sln dosyasını açın" -ForegroundColor Gray
    Write-Host "  2. Release x64 seçin" -ForegroundColor Gray
    Write-Host "  3. F5 ile çalıştırın" -ForegroundColor Gray
    Write-Host ""
}

# Ana akış
if ($Help) {
    Show-Help
    exit 0
}

Write-Banner

if (-not (Check-Prerequisites)) {
    exit 1
}

$vcpkgPath = ""
if (-not $SkipDependencies) {
    $vcpkgPath = Install-Vcpkg
    Install-Dependencies -vcpkgPath $vcpkgPath
}
else {
    $vcpkgPath = Join-Path $ProjectRoot "vcpkg"
    if (-not (Test-Path $vcpkgPath)) {
        Write-Warning "vcpkg bulunamadı, kurulum gerekebilir"
    }
    $env:VCPKG_ROOT = $vcpkgPath
}

if (-not $SkipModels) {
    Download-Models
}

if (Build-Project -IsDebug $Debug) {
    Run-Tests
    Show-Summary
}
else {
    Write-Error "Kurulum tamamlanamadı!"
    exit 1
}
