#pragma once
/**
 * @file setup_config.h
 * @brief Setup Konfigürasyonu - Kurulum sonrası ayarlar ve başlangıç yapılandırması
 */

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>

namespace Orora {
namespace Setup {

// Cinsiyet seçimi
enum class Gender {
    Female,
    Male,
    Neutral
};

// Ses tipi
enum class VoiceType {
    Default,            // Varsayılan ses
    CloneFromSample,    // Örnek sesten klonla
    CustomProfile       // Özel profil
};

// Dil
enum class Language {
    English,
    Turkish,
    German,
    French,
    Spanish,
    Japanese,
    Chinese,
    Arabic
};

// Başlangıç profili
struct InitialProfile {
    Gender gender = Gender::Female;
    VoiceType voiceType = VoiceType::Default;
    Language primaryLanguage = Language::Turkish;
    
    std::string customVoiceSamplePath;
    std::string customName;             // Kullanıcının ona vereceği ad (yoksa ORORA)
    
    bool enableCamera = true;
    bool enableMicrophone = true;
    bool enableMetahuman = true;
    bool enableRoom = true;
    
    bool runAtStartup = false;
    bool minimizeToTray = true;
    bool showWelcomeGuide = true;
};

// Sistem gereksinimleri
struct SystemRequirements {
    int minRAMGB = 8;
    int recommendedRAMGB = 16;
    int minDiskSpaceGB = 10;
    int recommendedDiskSpaceGB = 50;
    std::string minOS = "Windows 10 Build 17763";
    bool requiresGPU = false;
    bool requiresInternet = false;
};

// Sistem bilgisi
struct SystemInfo {
    int totalRAMGB;
    int availableDiskSpaceGB;
    std::string osVersion;
    std::string cpuName;
    std::string gpuName;
    bool hasNvidiaGPU;
    bool hasCUDA;
    bool hasCamera;
    bool hasMicrophone;
};

// Kurulum sonucu
struct SetupResult {
    bool success;
    std::string errorMessage;
    std::string configFilePath;
    InitialProfile profile;
};

/**
 * @class SetupConfig
 * @brief Kurulum yapılandırmasını yönetir
 */
class SetupConfig {
public:
    SetupConfig();
    ~SetupConfig();
    
    // ==================== Sistem Kontrolü ====================
    
    /**
     * @brief Sistem gereksinimlerini kontrol et
     */
    bool CheckSystemRequirements(std::vector<std::string>& issues);
    
    /**
     * @brief Sistem bilgisini al
     */
    SystemInfo GetSystemInfo() const;
    
    /**
     * @brief GPU kullanılabilir mi?
     */
    bool IsGPUAvailable() const;
    
    /**
     * @brief CUDA desteği var mı?
     */
    bool IsCUDAAvailable() const;
    
    /**
     * @brief Kamera var mı?
     */
    bool IsCameraAvailable() const;
    
    /**
     * @brief Mikrofon var mı?
     */
    bool IsMicrophoneAvailable() const;
    
    // ==================== Profil Yönetimi ====================
    
    /**
     * @brief Başlangıç profilini ayarla
     */
    void SetInitialProfile(const InitialProfile& profile);
    
    /**
     * @brief Başlangıç profilini al
     */
    InitialProfile GetInitialProfile() const;
    
    /**
     * @brief Profili dosyadan yükle
     */
    bool LoadProfile(const std::string& path);
    
    /**
     * @brief Profili dosyaya kaydet
     */
    bool SaveProfile(const std::string& path) const;
    
    /**
     * @brief INI dosyasından yükle (installer tarafından oluşturulan)
     */
    bool LoadFromInstallerConfig(const std::string& iniPath);
    
    // ==================== Kurulum İşlemleri ====================
    
    /**
     * @brief İlk kurulumu uygula
     */
    SetupResult ApplyInitialSetup();
    
    /**
     * @brief Varsayılan değerlerle kurulum
     */
    SetupResult ApplyDefaultSetup();
    
    /**
     * @brief Kurulum tamamlandı mı?
     */
    bool IsSetupCompleted() const;
    
    /**
     * @brief Kurulumu sıfırla
     */
    void ResetSetup();
    
    // ==================== Dizin Yönetimi ====================
    
    /**
     * @brief Uygulama dizini
     */
    std::string GetAppDirectory() const;
    
    /**
     * @brief Veri dizini
     */
    std::string GetDataDirectory() const;
    
    /**
     * @brief Model dizini
     */
    std::string GetModelsDirectory() const;
    
    /**
     * @brief Ses profilleri dizini
     */
    std::string GetVoiceProfilesDirectory() const;
    
    /**
     * @brief Log dizini
     */
    std::string GetLogDirectory() const;
    
    /**
     * @brief Export dizini
     */
    std::string GetExportDirectory() const;
    
    /**
     * @brief Tüm dizinleri oluştur
     */
    bool CreateDirectories();
    
    // ==================== Model İndirme ====================
    
    /**
     * @brief Gerekli modeller mevcut mu?
     */
    bool AreModelsAvailable() const;
    
    /**
     * @brief Eksik modelleri listele
     */
    std::vector<std::string> GetMissingModels() const;
    
    /**
     * @brief Model indirme URL'lerini al
     */
    std::unordered_map<std::string, std::string> GetModelDownloadURLs() const;
    
    // ==================== Yardımcılar ====================
    
    /**
     * @brief Cinsiyet adını al
     */
    static std::string GenderToString(Gender gender);
    
    /**
     * @brief Dil kodunu al
     */
    static std::string LanguageToCode(Language lang);
    
    /**
     * @brief Dil adını al
     */
    static std::string LanguageToString(Language lang);
    
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
    
    void DetectSystemCapabilities();
    bool CreateDirectory(const std::string& path);
};

/**
 * @class FirstRunWizard
 * @brief İlk çalıştırma sihirbazı
 */
class FirstRunWizard {
public:
    FirstRunWizard();
    ~FirstRunWizard();
    
    /**
     * @brief Wizard'ı başlat
     */
    bool Start();
    
    /**
     * @brief Wizard tamamlandı mı?
     */
    bool IsCompleted() const;
    
    /**
     * @brief Sonuç profilini al
     */
    InitialProfile GetResultProfile() const;
    
    // Wizard adımları
    void ShowWelcomeStep();
    void ShowGenderStep();
    void ShowVoiceStep();
    void ShowLanguageStep();
    void ShowFeaturesStep();
    void ShowConfirmationStep();
    void ShowCompletionStep();
    
    /**
     * @brief Mevcut adım
     */
    int GetCurrentStep() const;
    
    /**
     * @brief Toplam adım sayısı
     */
    int GetTotalSteps() const;
    
    /**
     * @brief Sonraki adıma geç
     */
    bool NextStep();
    
    /**
     * @brief Önceki adıma dön
     */
    bool PreviousStep();
    
    /**
     * @brief İptal et
     */
    void Cancel();
    
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace Setup
} // namespace Orora
