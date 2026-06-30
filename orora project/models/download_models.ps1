# ORORA AI - Model İndirme Scripti
# PowerShell 7+ gerektirir

param(
    [switch]$All,
    [switch]$Whisper,
    [switch]$VITS,
    [switch]$SpeakerEncoder,
    [switch]$Help
)

$ErrorActionPreference = "Stop"
$ProgressPreference = "Continue"

$ModelsDir = $PSScriptRoot
$VoiceDir = Join-Path $ModelsDir "voice"
$WhisperDir = Join-Path $VoiceDir "whisper"
$VITSDir = Join-Path $VoiceDir "vits"
$SpeakerDir = Join-Path $VoiceDir "speaker_encoder"

# Renk fonksiyonları
function Write-Success { param($msg) Write-Host "✅ $msg" -ForegroundColor Green }
function Write-Info { param($msg) Write-Host "ℹ️  $msg" -ForegroundColor Cyan }
function Write-Warning { param($msg) Write-Host "⚠️  $msg" -ForegroundColor Yellow }
function Write-Error { param($msg) Write-Host "❌ $msg" -ForegroundColor Red }

function Show-Help {
    Write-Host @"
ORORA AI - Model İndirme Scripti
================================

Kullanım:
  .\download_models.ps1 -All           # Tüm modelleri indir
  .\download_models.ps1 -Whisper       # Sadece Whisper STT
  .\download_models.ps1 -VITS          # Sadece VITS TTS
  .\download_models.ps1 -SpeakerEncoder # Sadece Speaker Encoder
  .\download_models.ps1 -Help          # Bu yardım mesajı

Model Boyutları:
  - Whisper (ggml-base.bin):  ~140 MB
  - Whisper (ggml-small.bin): ~480 MB
  - VITS Turkish:             ~50 MB
  - Speaker Encoder:          ~20 MB

"@
}

function Download-File {
    param(
        [string]$Url,
        [string]$OutputPath,
        [string]$DisplayName
    )
    
    if (Test-Path $OutputPath) {
        Write-Warning "$DisplayName zaten mevcut: $OutputPath"
        return $true
    }
    
    $dir = Split-Path $OutputPath -Parent
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }
    
    Write-Info "$DisplayName indiriliyor..."
    Write-Host "   URL: $Url" -ForegroundColor Gray
    Write-Host "   Hedef: $OutputPath" -ForegroundColor Gray
    
    try {
        # PowerShell 7+ ile ilerleme göstergesi
        $webClient = New-Object System.Net.WebClient
        $webClient.DownloadFile($Url, $OutputPath)
        
        $size = (Get-Item $OutputPath).Length / 1MB
        Write-Success "$DisplayName indirildi ({0:N2} MB)" -f $size
        return $true
    }
    catch {
        Write-Error "$DisplayName indirilemedi: $_"
        return $false
    }
}

function Download-Whisper {
    Write-Host "`n📥 Whisper STT Modelleri İndiriliyor..." -ForegroundColor Magenta
    
    $models = @{
        "ggml-base.bin" = "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.bin"
        # "ggml-small.bin" = "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-small.bin"
    }
    
    foreach ($model in $models.GetEnumerator()) {
        $outputPath = Join-Path $WhisperDir $model.Key
        Download-File -Url $model.Value -OutputPath $outputPath -DisplayName "Whisper $($model.Key)"
    }
}

function Download-VITS {
    Write-Host "`n📥 VITS TTS Modeli İndiriliyor..." -ForegroundColor Magenta
    
    # VITS modeli için placeholder - gerçek model ayrıca eğitilmeli
    Write-Warning "VITS Türkçe modeli henüz hazır değil."
    Write-Info "Alternatif: SileroTTS veya espeak-ng kullanılabilir."
    
    # Silero TTS modeli (Türkçe destekli)
    $sileroUrl = "https://models.silero.ai/models/tts/tr/v3_tr.pt"
    $sileroPath = Join-Path $VITSDir "silero_tr_v3.pt"
    
    Download-File -Url $sileroUrl -OutputPath $sileroPath -DisplayName "Silero TTS (Türkçe)"
}

function Download-SpeakerEncoder {
    Write-Host "`n📥 Speaker Encoder Modeli İndiriliyor..." -ForegroundColor Magenta
    
    # Resemblyzer / ECAPA-TDNN modeli
    $url = "https://github.com/resemble-ai/Resemblyzer/raw/master/pretrained/encoder.pt"
    $outputPath = Join-Path $SpeakerDir "encoder.pt"
    
    Download-File -Url $url -OutputPath $outputPath -DisplayName "Speaker Encoder"
}

function Create-Directories {
    $dirs = @($WhisperDir, $VITSDir, $SpeakerDir)
    foreach ($dir in $dirs) {
        if (-not (Test-Path $dir)) {
            New-Item -ItemType Directory -Path $dir -Force | Out-Null
            Write-Info "Klasör oluşturuldu: $dir"
        }
    }
}

# Ana akış
if ($Help) {
    Show-Help
    exit 0
}

Write-Host @"
╔══════════════════════════════════════════════════╗
║           ORORA AI - Model İndirici              ║
╚══════════════════════════════════════════════════╝
"@ -ForegroundColor Cyan

Create-Directories

if ($All -or (-not $Whisper -and -not $VITS -and -not $SpeakerEncoder)) {
    Download-Whisper
    Download-VITS
    Download-SpeakerEncoder
}
else {
    if ($Whisper) { Download-Whisper }
    if ($VITS) { Download-VITS }
    if ($SpeakerEncoder) { Download-SpeakerEncoder }
}

Write-Host "`n" 
Write-Success "Model indirme tamamlandı!"
Write-Host ""
