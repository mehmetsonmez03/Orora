# ORORA AI Models Directory

Bu klasör AI model dosyalarını içerir. Aşağıdaki dosyalar gereklidir:

## Voice Models (`voice/`)

### Whisper STT
```
voice/whisper/
├── ggml-base.bin          # ~140 MB - Temel model
├── ggml-small.bin         # ~480 MB - Önerilen
└── ggml-medium.bin        # ~1.5 GB - Yüksek doğruluk
```

İndirme: https://huggingface.co/ggerganov/whisper.cpp/tree/main

### VITS TTS
```
voice/vits/
├── vits_turkish.onnx      # Türkçe VITS modeli
├── config.json            # Model yapılandırması
└── vocab.json             # Karakter tablosu
```

### Speaker Encoder
```
voice/speaker_encoder/
├── encoder.onnx           # Speaker embedding modeli
└── config.json
```

## Metahuman Models (`metahuman/`)

### Karakter
```
metahuman/
├── orora.fbx              # Ana karakter modeli
└── textures/
    ├── body_basecolor.png
    ├── body_normal.png
    ├── face_basecolor.png
    ├── face_normal.png
    ├── eyes_basecolor.png
    └── hair_basecolor.png
```

### Saç Stilleri (`hair/`)
```
hair/
├── long_straight.fbx
├── long_wavy.fbx
├── medium_straight.fbx
├── short_straight.fbx
├── bob.fbx
├── ponytail.fbx
└── ...
```

### Kıyafetler (`clothing/`)
```
clothing/
├── tops/
│   ├── white_tshirt.fbx
│   ├── black_blouse.fbx
│   └── ...
├── bottoms/
│   ├── blue_jeans.fbx
│   ├── black_skirt.fbx
│   └── ...
├── dresses/
│   ├── red_dress.fbx
│   └── ...
└── sleepwear/
    ├── pink_pajamas.fbx
    └── ...
```

## Animations (`animations/`)
```
animations/
├── idle_standing.fbx
├── walking.fbx
├── sitting_relaxed.fbx
├── sleeping.fbx
├── reading.fbx
└── ...
```

## Dosya Boyutları (Tahmini)

| Kategori | Boyut |
|----------|-------|
| Voice Models | ~500 MB - 2 GB |
| Metahuman | ~100 MB - 500 MB |
| Hair/Clothing | ~50 MB - 200 MB |
| Animations | ~20 MB - 50 MB |
| **TOPLAM** | **~700 MB - 3 GB** |

## Lisanslar

- Whisper: MIT License
- VITS: Apache 2.0
- Metahuman modelleri: Kullanıcı tarafından sağlanmalı
