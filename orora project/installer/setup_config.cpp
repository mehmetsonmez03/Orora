#include "setup_config.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#include <ShlObj.h>
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#endif

namespace fs = std::filesystem;

namespace Orora {
namespace Setup {

struct SetupConfig::Impl {
    InitialProfile profile;
    SystemInfo systemInfo;
    bool setupCompleted = false;
    std::string appDirectory;
    std::string dataDirectory;
};

SetupConfig::SetupConfig() : pImpl(std::make_unique<Impl>()) {
    DetectSystemCapabilities();
    
    // Varsayılan dizinler
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, path))) {
        pImpl->appDirectory = std::string(path) + "\\ORORA";
    }
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        pImpl->dataDirectory = std::string(path) + "\\ORORA";
    }
#else
    pImpl->appDirectory = "/opt/orora";
    pImpl->dataDirectory = std::string(getenv("HOME")) + "/.orora";
#endif
}

SetupConfig::~SetupConfig() = default;

// ==================== Sistem Kontrolü ====================

bool SetupConfig::CheckSystemRequirements(std::vector<std::string>& issues) {
    issues.clear();
    SystemRequirements req;
    
    if (pImpl->systemInfo.totalRAMGB < req.minRAMGB) {
        issues.push_back("Insufficient RAM: " + std::to_string(pImpl->systemInfo.totalRAMGB) +
                         "GB available, " + std::to_string(req.minRAMGB) + "GB required");
    }
    
    if (pImpl->systemInfo.availableDiskSpaceGB < req.minDiskSpaceGB) {
        issues.push_back("Insufficient disk space: " + 
                         std::to_string(pImpl->systemInfo.availableDiskSpaceGB) + "GB available, " +
                         std::to_string(req.minDiskSpaceGB) + "GB required");
    }
    
    return issues.empty();
}

SystemInfo SetupConfig::GetSystemInfo() const {
    return pImpl->systemInfo;
}

bool SetupConfig::IsGPUAvailable() const {
    return pImpl->systemInfo.hasNvidiaGPU;
}

bool SetupConfig::IsCUDAAvailable() const {
    return pImpl->systemInfo.hasCUDA;
}

bool SetupConfig::IsCameraAvailable() const {
    return pImpl->systemInfo.hasCamera;
}

bool SetupConfig::IsMicrophoneAvailable() const {
    return pImpl->systemInfo.hasMicrophone;
}

void SetupConfig::DetectSystemCapabilities() {
#ifdef _WIN32
    // RAM
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    pImpl->systemInfo.totalRAMGB = static_cast<int>(memInfo.ullTotalPhys / (1024 * 1024 * 1024));
    
    // Disk
    ULARGE_INTEGER freeBytesAvailable;
    GetDiskFreeSpaceExW(L"C:\\", &freeBytesAvailable, NULL, NULL);
    pImpl->systemInfo.availableDiskSpaceGB = static_cast<int>(freeBytesAvailable.QuadPart / (1024 * 1024 * 1024));
    
    // OS Version
    OSVERSIONINFOEXW osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
    // GetVersionExW deprecated but still works
    pImpl->systemInfo.osVersion = "Windows 10+";
    
    // GPU
    IDXGIFactory* pFactory = nullptr;
    if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
        IDXGIAdapter* pAdapter = nullptr;
        if (SUCCEEDED(pFactory->EnumAdapters(0, &pAdapter))) {
            DXGI_ADAPTER_DESC desc;
            pAdapter->GetDesc(&desc);
            
            char gpuName[128];
            WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, gpuName, 128, NULL, NULL);
            pImpl->systemInfo.gpuName = gpuName;
            
            // NVIDIA kontrolü
            std::string gpuLower = pImpl->systemInfo.gpuName;
            std::transform(gpuLower.begin(), gpuLower.end(), gpuLower.begin(), ::tolower);
            pImpl->systemInfo.hasNvidiaGPU = gpuLower.find("nvidia") != std::string::npos;
            
            pAdapter->Release();
        }
        pFactory->Release();
    }
    
    // CUDA kontrolü (basit)
    pImpl->systemInfo.hasCUDA = pImpl->systemInfo.hasNvidiaGPU;
    
    // Kamera ve mikrofon (basit kontrol)
    pImpl->systemInfo.hasCamera = true;  // Gerçekte DeviceEnumeration ile kontrol edilmeli
    pImpl->systemInfo.hasMicrophone = true;
#else
    pImpl->systemInfo.totalRAMGB = 16;
    pImpl->systemInfo.availableDiskSpaceGB = 100;
    pImpl->systemInfo.osVersion = "Linux";
    pImpl->systemInfo.hasNvidiaGPU = false;
    pImpl->systemInfo.hasCUDA = false;
    pImpl->systemInfo.hasCamera = true;
    pImpl->systemInfo.hasMicrophone = true;
#endif
}

// ==================== Profil Yönetimi ====================

void SetupConfig::SetInitialProfile(const InitialProfile& profile) {
    pImpl->profile = profile;
}

InitialProfile SetupConfig::GetInitialProfile() const {
    return pImpl->profile;
}

bool SetupConfig::LoadProfile(const std::string& path) {
    std::ifstream file(path);
    if (!file) return false;
    
    // Basit JSON okuma (gerçekte bir JSON kütüphanesi kullanılmalı)
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    
    // Parse (placeholder)
    return true;
}

bool SetupConfig::SaveProfile(const std::string& path) const {
    std::ofstream file(path);
    if (!file) return false;
    
    file << "{\n";
    file << "  \"gender\": \"" << GenderToString(pImpl->profile.gender) << "\",\n";
    file << "  \"voiceType\": " << static_cast<int>(pImpl->profile.voiceType) << ",\n";
    file << "  \"language\": \"" << LanguageToCode(pImpl->profile.primaryLanguage) << "\",\n";
    file << "  \"enableCamera\": " << (pImpl->profile.enableCamera ? "true" : "false") << ",\n";
    file << "  \"enableMicrophone\": " << (pImpl->profile.enableMicrophone ? "true" : "false") << ",\n";
    file << "  \"enableMetahuman\": " << (pImpl->profile.enableMetahuman ? "true" : "false") << ",\n";
    file << "  \"enableRoom\": " << (pImpl->profile.enableRoom ? "true" : "false") << "\n";
    file << "}\n";
    
    return true;
}

bool SetupConfig::LoadFromInstallerConfig(const std::string& iniPath) {
    std::ifstream file(iniPath);
    if (!file) return false;
    
    std::string line;
    std::string section;
    
    while (std::getline(file, line)) {
        // Trim
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (line.empty() || line[0] == ';') continue;
        
        if (line[0] == '[') {
            section = line.substr(1, line.find(']') - 1);
            continue;
        }
        
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        
        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        if (section == "Identity") {
            if (key == "Gender") {
                if (value == "Female") pImpl->profile.gender = Gender::Female;
                else if (value == "Male") pImpl->profile.gender = Gender::Male;
                else pImpl->profile.gender = Gender::Neutral;
            }
        } else if (section == "Voice") {
            if (key == "Type") {
                if (value == "Default") pImpl->profile.voiceType = VoiceType::Default;
                else if (value == "CloneLater") pImpl->profile.voiceType = VoiceType::CloneFromSample;
            }
        } else if (section == "Language") {
            if (key == "Primary") {
                if (value == "en") pImpl->profile.primaryLanguage = Language::English;
                else if (value == "tr") pImpl->profile.primaryLanguage = Language::Turkish;
                else if (value == "de") pImpl->profile.primaryLanguage = Language::German;
                else if (value == "fr") pImpl->profile.primaryLanguage = Language::French;
                else if (value == "es") pImpl->profile.primaryLanguage = Language::Spanish;
            }
        }
    }
    
    return true;
}

// ==================== Kurulum İşlemleri ====================

SetupResult SetupConfig::ApplyInitialSetup() {
    SetupResult result;
    result.success = true;
    result.profile = pImpl->profile;
    
    // Dizinleri oluştur
    if (!CreateDirectories()) {
        result.success = false;
        result.errorMessage = "Failed to create directories";
        return result;
    }
    
    // Profili kaydet
    result.configFilePath = GetDataDirectory() + "/profile.json";
    if (!SaveProfile(result.configFilePath)) {
        result.success = false;
        result.errorMessage = "Failed to save profile";
        return result;
    }
    
    pImpl->setupCompleted = true;
    
    return result;
}

SetupResult SetupConfig::ApplyDefaultSetup() {
    InitialProfile defaultProfile;
    SetInitialProfile(defaultProfile);
    return ApplyInitialSetup();
}

bool SetupConfig::IsSetupCompleted() const {
    return pImpl->setupCompleted;
}

void SetupConfig::ResetSetup() {
    pImpl->setupCompleted = false;
    pImpl->profile = InitialProfile{};
}

// ==================== Dizin Yönetimi ====================

std::string SetupConfig::GetAppDirectory() const {
    return pImpl->appDirectory;
}

std::string SetupConfig::GetDataDirectory() const {
    return pImpl->dataDirectory;
}

std::string SetupConfig::GetModelsDirectory() const {
    return pImpl->appDirectory + "/models";
}

std::string SetupConfig::GetVoiceProfilesDirectory() const {
    return pImpl->dataDirectory + "/voice_profiles";
}

std::string SetupConfig::GetLogDirectory() const {
    return pImpl->dataDirectory + "/logs";
}

std::string SetupConfig::GetExportDirectory() const {
    return pImpl->dataDirectory + "/exports";
}

bool SetupConfig::CreateDirectories() {
    std::vector<std::string> dirs = {
        GetDataDirectory(),
        GetVoiceProfilesDirectory(),
        GetLogDirectory(),
        GetExportDirectory(),
        GetDataDirectory() + "/memory",
        GetDataDirectory() + "/profiles"
    };
    
    for (const auto& dir : dirs) {
        if (!CreateDirectory(dir)) {
            return false;
        }
    }
    
    return true;
}

bool SetupConfig::CreateDirectory(const std::string& path) {
    try {
        fs::create_directories(path);
        return true;
    } catch (...) {
        return false;
    }
}

// ==================== Model İndirme ====================

bool SetupConfig::AreModelsAvailable() const {
    std::vector<std::string> requiredModels = {
        GetModelsDirectory() + "/voice/vits.onnx",
        GetModelsDirectory() + "/voice/whisper.onnx"
    };
    
    for (const auto& model : requiredModels) {
        if (!fs::exists(model)) {
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> SetupConfig::GetMissingModels() const {
    std::vector<std::string> missing;
    
    std::vector<std::pair<std::string, std::string>> requiredModels = {
        {"vits.onnx", GetModelsDirectory() + "/voice/vits.onnx"},
        {"whisper.onnx", GetModelsDirectory() + "/voice/whisper.onnx"},
        {"face_detector.onnx", GetModelsDirectory() + "/vision/face_detector.onnx"},
        {"emotion.onnx", GetModelsDirectory() + "/vision/emotion.onnx"}
    };
    
    for (const auto& model : requiredModels) {
        if (!fs::exists(model.second)) {
            missing.push_back(model.first);
        }
    }
    
    return missing;
}

std::unordered_map<std::string, std::string> SetupConfig::GetModelDownloadURLs() const {
    return {
        {"vits.onnx", "https://models.orora.ai/voice/vits_v1.onnx"},
        {"whisper.onnx", "https://models.orora.ai/voice/whisper_base.onnx"},
        {"face_detector.onnx", "https://models.orora.ai/vision/scrfd_10g.onnx"},
        {"emotion.onnx", "https://models.orora.ai/vision/emotion_fer.onnx"}
    };
}

// ==================== Yardımcılar ====================

std::string SetupConfig::GenderToString(Gender gender) {
    switch (gender) {
        case Gender::Female: return "Female";
        case Gender::Male: return "Male";
        default: return "Neutral";
    }
}

std::string SetupConfig::LanguageToCode(Language lang) {
    switch (lang) {
        case Language::English: return "en";
        case Language::Turkish: return "tr";
        case Language::German: return "de";
        case Language::French: return "fr";
        case Language::Spanish: return "es";
        case Language::Japanese: return "ja";
        case Language::Chinese: return "zh";
        case Language::Arabic: return "ar";
        default: return "en";
    }
}

std::string SetupConfig::LanguageToString(Language lang) {
    switch (lang) {
        case Language::English: return "English";
        case Language::Turkish: return "Turkish";
        case Language::German: return "German";
        case Language::French: return "French";
        case Language::Spanish: return "Spanish";
        case Language::Japanese: return "Japanese";
        case Language::Chinese: return "Chinese";
        case Language::Arabic: return "Arabic";
        default: return "English";
    }
}

// ==================== FirstRunWizard ====================

struct FirstRunWizard::Impl {
    int currentStep = 0;
    int totalSteps = 7;
    bool completed = false;
    bool cancelled = false;
    InitialProfile resultProfile;
};

FirstRunWizard::FirstRunWizard() : pImpl(std::make_unique<Impl>()) {}
FirstRunWizard::~FirstRunWizard() = default;

bool FirstRunWizard::Start() {
    pImpl->currentStep = 0;
    pImpl->completed = false;
    pImpl->cancelled = false;
    ShowWelcomeStep();
    return true;
}

bool FirstRunWizard::IsCompleted() const {
    return pImpl->completed;
}

InitialProfile FirstRunWizard::GetResultProfile() const {
    return pImpl->resultProfile;
}

int FirstRunWizard::GetCurrentStep() const {
    return pImpl->currentStep;
}

int FirstRunWizard::GetTotalSteps() const {
    return pImpl->totalSteps;
}

bool FirstRunWizard::NextStep() {
    if (pImpl->currentStep < pImpl->totalSteps - 1) {
        pImpl->currentStep++;
        
        switch (pImpl->currentStep) {
            case 1: ShowGenderStep(); break;
            case 2: ShowVoiceStep(); break;
            case 3: ShowLanguageStep(); break;
            case 4: ShowFeaturesStep(); break;
            case 5: ShowConfirmationStep(); break;
            case 6: ShowCompletionStep(); pImpl->completed = true; break;
        }
        
        return true;
    }
    return false;
}

bool FirstRunWizard::PreviousStep() {
    if (pImpl->currentStep > 0) {
        pImpl->currentStep--;
        return true;
    }
    return false;
}

void FirstRunWizard::Cancel() {
    pImpl->cancelled = true;
}

void FirstRunWizard::ShowWelcomeStep() {
    // Qt UI ile gösterilecek
}

void FirstRunWizard::ShowGenderStep() {
    // Qt UI ile gösterilecek
}

void FirstRunWizard::ShowVoiceStep() {
    // Qt UI ile gösterilecek
}

void FirstRunWizard::ShowLanguageStep() {
    // Qt UI ile gösterilecek
}

void FirstRunWizard::ShowFeaturesStep() {
    // Qt UI ile gösterilecek
}

void FirstRunWizard::ShowConfirmationStep() {
    // Qt UI ile gösterilecek
}

void FirstRunWizard::ShowCompletionStep() {
    // Qt UI ile gösterilecek
}

} // namespace Setup
} // namespace Orora
