# ORORA AI - Kurulum Kılavuzu

## 📋 Gereksinimler

### Zorunlu
- **Windows 10/11** 64-bit
- **Visual Studio 2022** Community/Professional
  - C++ Desktop Development workload
  - Windows 10/11 SDK
- **CMake 3.20+** ([İndir](https://cmake.org/download/))
- **Git** ([İndir](https://git-scm.com/download/win))

### Disk Alanı
| Bileşen | Boyut |
|---------|-------|
| Kaynak kod | ~50 MB |
| vcpkg + bağımlılıklar | ~2 GB |
| AI Modelleri | ~500 MB |
| Build çıktısı | ~500 MB |
| **Toplam** | **~3 GB** |

---

## 🚀 Kurulum Adımları

### Yöntem 1: Otomatik (Önerilen)

```powershell
# PowerShell'i yönetici olarak açın
cd "c:\orora project"
.\setup.ps1
```

Bu script:
1. vcpkg'yi kurar
2. Qt6, Assimp ve diğer bağımlılıkları yükler
3. AI modellerini indirir
4. Projeyi derler
5. Testleri çalıştırır

### Yöntem 2: Manuel

#### 1. vcpkg Kurulumu

```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
$env:VCPKG_ROOT = "$PWD"
```

#### 2. Bağımlılıkları Yükle

```powershell
.\vcpkg install qt6:x64-windows
.\vcpkg install assimp:x64-windows
.\vcpkg install nlohmann-json:x64-windows
.\vcpkg install glm:x64-windows
```

#### 3. ONNX Runtime

```powershell
# Manuel indirme
# https://github.com/microsoft/onnxruntime/releases
# onnxruntime-win-x64-1.16.3.zip indir
# external/onnxruntime klasörüne çıkar
```

#### 4. AI Modelleri

```powershell
.\models\download_models.ps1 -All
```

#### 5. Derleme

```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
```

---

## 🔧 Visual Studio ile

1. **orora/ORORA_Core.sln** dosyasını açın
2. Solution Configuration: **Release**
3. Solution Platform: **x64**
4. Build > Build Solution (Ctrl+Shift+B)
5. Debug > Start Without Debugging (Ctrl+F5)

---

## ✅ Doğrulama

```powershell
# Testleri çalıştır
cd build
ctest --output-on-failure

# Uygulamayı başlat
.\bin\Release\orora.exe
```

---

## ❓ Sorun Giderme

### "Qt6 bulunamadı"
```powershell
$env:Qt6_DIR = "$env:VCPKG_ROOT\installed\x64-windows\share\qt6"
```

### "ONNX Runtime bulunamadı"
- `external/onnxruntime/` klasörünü kontrol edin
- ONNX Runtime'ı manuel indirin

### "Whisper model yüklenemedi"
- `models/voice/whisper/ggml-base.bin` dosyasını kontrol edin
- ~141 MB olmalı

### Build hataları
```powershell
# Temiz build
.\build.ps1 -Clean -Configuration Release
```

---

## 📞 Destek

Sorunlar için GitHub Issues kullanın.
