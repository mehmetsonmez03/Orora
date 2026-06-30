# ORORA AI

<p align="center">
  <img src="docs/images/orora_logo.png" alt="ORORA Logo" width="200"/>
</p>

<p align="center">
  <strong>Bilinçli Yapay Zeka Asistanı</strong><br>
  İnsan beynine benzer bilinç sistemiyle çalışan, tamamen C++ native AI
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg" alt="C++20"/>
  <img src="https://img.shields.io/badge/Qt-6.x-green.svg" alt="Qt6"/>
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="MIT"/>
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey.svg" alt="Windows"/>
</p>

---

## 🌟 Özellikler

### 🧠 Bilinç Sistemi
- İnsan beynine benzer lob mimarisi (Frontal, Temporal, Parietal, Occipital)
- Etik süzgeç ve değer sistemi
- Öz-programlama ve modül değiştirme
- Öngörü ve senaryo simülasyonu

### 🗣️ Ses Sistemi (Native C++)
- **STT**: Whisper.cpp ile gerçek zamanlı konuşma tanıma
- **TTS**: VITS/ONNX ile doğal ses sentezi
- **Voice Cloning**: Herhangi bir sesi klonlama yeteneği

### 👩 Metahuman Render
- Unreal Engine Metahuman FBX desteği
- 125+ blend shape ile yüz ifadeleri
- Skeletal animation + Inverse Kinematics
- Gerçek zamanlı lip sync (Türkçe phoneme)

### 🏠 3D Oda Sistemi
- Etkileşimli mobilyalar (yatak, dolap, koltuk, masa)
- A* pathfinding ile akıllı gezinme
- Gün/gece döngüsü ve aydınlatma

### 👗 Görünüm Özelleştirme
- 16 saç rengi ve 16 saç stili
- Kıyafet kataloğu ve outfit sistemi
- Gerçek zamanlı renk değiştirme

### 💬 Kullanıcı Arayüzü
- Qt6 tabanlı modern UI
- Sohbet paneli (dark/light tema)
- Dosya oluşturma ve indirme

---

## 🚀 Hızlı Başlangıç

### Gereksinimler
- Windows 10/11 64-bit
- Visual Studio 2022 (C++ Desktop Development)
- CMake 3.20+
- Git

### Otomatik Kurulum

```powershell
# Projeyi klonlayın
git clone https://github.com/orora-ai/orora.git
cd orora

# Otomatik kurulum (bağımlılıklar + derleme + modeller)
.\setup.ps1
```

### Manuel Kurulum

```powershell
# 1. vcpkg kurulumu
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
$env:VCPKG_ROOT = "$PWD\vcpkg"

# 2. Bağımlılıklar
.\vcpkg\vcpkg install qt6:x64-windows assimp:x64-windows

# 3. AI Modelleri
.\models\download_models.ps1 -All

# 4. Derleme
cmake -B build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release

# 5. Çalıştır
.\build\bin\orora.exe
```

---

## 📁 Proje Yapısı

```
orora/
├── orora/
│   ├── voice_system/       # STT, TTS, Voice Cloning
│   ├── metahuman/          # 3D render, animation
│   │   ├── rendering/      # OpenGL, shaders
│   │   ├── room/           # 3D oda, navigasyon
│   │   └── appearance/     # Saç, kıyafet
│   ├── orora_core/         # Bilinç sistemi
│   ├── ui/                 # Qt6 arayüz
│   └── orora_app.cpp       # Ana uygulama
├── models/                 # AI model dosyaları
├── tests/                  # Birim testleri
├── CMakeLists.txt
├── vcpkg.json
└── setup.ps1
```

---

## 🛠️ Geliştirme

### Visual Studio ile
1. `orora/ORORA_Core.sln` dosyasını açın
2. Release x64 yapılandırmasını seçin
3. F5 ile çalıştırın

### CMake ile
```powershell
cmake --preset debug
cmake --build build/debug
ctest --test-dir build/debug
```

### Testler
```powershell
cd build
ctest --output-on-failure
```

---

## 📊 Performans

| Metrik | Değer |
|--------|-------|
| İlk başlatma | ~3 saniye |
| Konuşma tanıma | <500ms |
| Ses sentezi | <200ms |
| Yüz animasyonu | 60 FPS |
| RAM kullanımı | ~500 MB |

---

## 🤝 Katkıda Bulunma

1. Fork yapın
2. Feature branch oluşturun (`git checkout -b feature/amazing`)
3. Commit yapın (`git commit -m 'Add amazing feature'`)
4. Push yapın (`git push origin feature/amazing`)
5. Pull Request açın

---

## 📄 Lisans

MIT License - detaylar için [LICENSE](LICENSE) dosyasına bakın.

---

## 🙏 Teşekkürler

- [Whisper.cpp](https://github.com/ggerganov/whisper.cpp) - STT
- [VITS](https://github.com/jaywalnut310/vits) - TTS mimarisi
- [Assimp](https://github.com/assimp/assimp) - 3D model yükleme
- [Qt](https://www.qt.io/) - UI framework
