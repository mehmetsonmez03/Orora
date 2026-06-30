; ORORA Installer Script for Inno Setup
; Version: 1.0.0
; Description: ORORA - Advanced Artificial Consciousness System

#define MyAppName "ORORA"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "ORORA Project"
#define MyAppURL "https://orora.ai"
#define MyAppExeName "ORORA.exe"
#define MyAppDescription "Advanced Artificial Consciousness System"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
AppId={{A4B5C6D7-E8F9-0A1B-2C3D-4E5F6A7B8C9D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}/support
AppUpdatesURL={#MyAppURL}/updates
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=..\LICENSE
InfoBeforeFile=..\docs\INSTALLATION.md
OutputDir=..\build\installer
OutputBaseFilename=ORORA_Setup_{#MyAppVersion}
SetupIconFile=resources\icon.ico
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
WizardResizable=yes
PrivilegesRequired=admin
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
MinVersion=10.0.17763

; Branding
WizardImageFile=resources\wizard_image.bmp
WizardSmallImageFile=resources\wizard_small.bmp

; Uninstaller
UninstallDisplayIcon={app}\{#MyAppExeName}
UninstallDisplayName={#MyAppName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"

[Types]
Name: "full"; Description: "Full Installation (Recommended)"
Name: "compact"; Description: "Compact Installation"
Name: "custom"; Description: "Custom Installation"; Flags: iscustom

[Components]
Name: "main"; Description: "ORORA Core System"; Types: full compact custom; Flags: fixed
Name: "models"; Description: "AI Models (Required for offline use)"; Types: full
Name: "models\vision"; Description: "Vision Models (Face Detection, Emotion)"; Types: full
Name: "models\voice"; Description: "Voice Models (TTS, STT)"; Types: full
Name: "avatar"; Description: "Metahuman Avatar"; Types: full
Name: "avatar\female"; Description: "Female Avatar"; Types: full; Flags: exclusive
Name: "avatar\male"; Description: "Male Avatar"; Types: full; Flags: exclusive
Name: "room"; Description: "Virtual Room Environment"; Types: full

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 6.1; Check: not IsAdminInstallMode
Name: "startup"; Description: "Start ORORA with Windows"; Flags: unchecked

[Files]
; Main executable
Source: "..\build\Release\ORORA.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main

; Qt DLLs
Source: "..\orora\Qt6Core.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\orora\Qt6Gui.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\orora\Qt6Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: main

; Qt Platforms
Source: "..\build\Release\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs; Components: main

; ONNX Runtime
Source: "..\build\Release\onnxruntime.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: main

; Data files
Source: "..\data\*"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: main

; Vision Models
Source: "..\models\vision\*"; DestDir: "{app}\models\vision"; Flags: ignoreversion recursesubdirs; Components: models\vision

; Voice Models
Source: "..\models\voice\*"; DestDir: "{app}\models\voice"; Flags: ignoreversion recursesubdirs; Components: models\voice

; Metahuman (Female)
Source: "..\orora\metahuman\orora.fbx"; DestDir: "{app}\metahuman"; Flags: ignoreversion; Components: avatar\female
Source: "..\models\metahuman\female\*"; DestDir: "{app}\metahuman\textures"; Flags: ignoreversion recursesubdirs; Components: avatar\female

; Metahuman (Male)  
Source: "..\models\metahuman\male\*"; DestDir: "{app}\metahuman\textures"; Flags: ignoreversion recursesubdirs; Components: avatar\male

; Room environment
Source: "..\models\room\*"; DestDir: "{app}\room"; Flags: ignoreversion recursesubdirs; Components: room

; Documentation
Source: "..\docs\*"; DestDir: "{app}\docs"; Flags: ignoreversion recursesubdirs; Components: main

; License
Source: "..\LICENSE"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\README.md"; DestDir: "{app}"; Flags: ignoreversion; Components: main

[Dirs]
Name: "{app}\logs"; Permissions: users-modify
Name: "{app}\exports"; Permissions: users-modify
Name: "{app}\memory"; Permissions: users-modify
Name: "{app}\profiles"; Permissions: users-modify
Name: "{app}\voice_profiles"; Permissions: users-modify

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
; First run wizard
Filename: "{app}\{#MyAppExeName}"; Parameters: "--setup-wizard"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallRun]
Filename: "{app}\{#MyAppExeName}"; Parameters: "--cleanup"; Flags: skipifnotsilent

[Registry]
; File associations (optional)
Root: HKCR; Subkey: ".orora"; ValueType: string; ValueName: ""; ValueData: "ORORA.Project"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "ORORA.Project"; ValueType: string; ValueName: ""; ValueData: "ORORA Project File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "ORORA.Project\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},0"
Root: HKCR; Subkey: "ORORA.Project\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""

; Startup entry (if selected)
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "ORORA"; ValueData: """{app}\{#MyAppExeName}"" --minimized"; Flags: uninsdeletevalue; Tasks: startup

[Code]
var
  GenderPage: TInputOptionWizardPage;
  VoicePage: TInputOptionWizardPage;
  LanguagePage: TInputOptionWizardPage;
  SelectedGender: Integer;
  SelectedVoice: Integer;
  SelectedLanguage: Integer;

procedure InitializeWizard;
begin
  // Gender Selection Page
  GenderPage := CreateInputOptionPage(wpSelectComponents,
    'Select Gender', 'Choose ORORA''s personality and appearance',
    'ORORA will adapt its consciousness, voice, and appearance based on your selection.',
    True, False);
  GenderPage.Add('Female - Feminine consciousness and appearance');
  GenderPage.Add('Male - Masculine consciousness and appearance');
  GenderPage.Values[0] := True;
  
  // Voice Selection Page
  VoicePage := CreateInputOptionPage(GenderPage.ID,
    'Voice Configuration', 'Configure ORORA''s voice',
    'You can customize the voice later or record a reference voice for cloning.',
    True, False);
  VoicePage.Add('Default Voice - Use the default high-quality voice');
  VoicePage.Add('Clone Voice Later - Set up voice cloning after installation');
  VoicePage.Values[0] := True;
  
  // Language Selection Page
  LanguagePage := CreateInputOptionPage(VoicePage.ID,
    'Primary Language', 'Select ORORA''s primary language',
    'ORORA will primarily communicate in this language (can be changed later).',
    True, False);
  LanguagePage.Add('English');
  LanguagePage.Add('Turkish');
  LanguagePage.Add('German');
  LanguagePage.Add('French');
  LanguagePage.Add('Spanish');
  LanguagePage.Values[1] := True; // Default to Turkish
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;
  
  if CurPageID = GenderPage.ID then
  begin
    if GenderPage.Values[0] then
      SelectedGender := 0  // Female
    else
      SelectedGender := 1; // Male
  end;
  
  if CurPageID = VoicePage.ID then
  begin
    if VoicePage.Values[0] then
      SelectedVoice := 0   // Default
    else
      SelectedVoice := 1;  // Clone later
  end;
  
  if CurPageID = LanguagePage.ID then
  begin
    if LanguagePage.Values[0] then SelectedLanguage := 0
    else if LanguagePage.Values[1] then SelectedLanguage := 1
    else if LanguagePage.Values[2] then SelectedLanguage := 2
    else if LanguagePage.Values[3] then SelectedLanguage := 3
    else if LanguagePage.Values[4] then SelectedLanguage := 4;
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  ConfigFile: String;
  ConfigContent: String;
begin
  if CurStep = ssPostInstall then
  begin
    // Save configuration
    ConfigFile := ExpandConstant('{app}\config\initial_setup.ini');
    
    ConfigContent := '[Identity]' + #13#10;
    if SelectedGender = 0 then
      ConfigContent := ConfigContent + 'Gender=Female' + #13#10
    else
      ConfigContent := ConfigContent + 'Gender=Male' + #13#10;
    
    ConfigContent := ConfigContent + #13#10 + '[Voice]' + #13#10;
    if SelectedVoice = 0 then
      ConfigContent := ConfigContent + 'Type=Default' + #13#10
    else
      ConfigContent := ConfigContent + 'Type=CloneLater' + #13#10;
    
    ConfigContent := ConfigContent + #13#10 + '[Language]' + #13#10;
    case SelectedLanguage of
      0: ConfigContent := ConfigContent + 'Primary=en' + #13#10;
      1: ConfigContent := ConfigContent + 'Primary=tr' + #13#10;
      2: ConfigContent := ConfigContent + 'Primary=de' + #13#10;
      3: ConfigContent := ConfigContent + 'Primary=fr' + #13#10;
      4: ConfigContent := ConfigContent + 'Primary=es' + #13#10;
    end;
    
    ForceDirectories(ExpandConstant('{app}\config'));
    SaveStringToFile(ConfigFile, ConfigContent, False);
  end;
end;

function InitializeSetup(): Boolean;
begin
  Result := True;
  
  // Check for Windows 10 or later
  if not IsWindows10OrNewer() then
  begin
    MsgBox('ORORA requires Windows 10 or later.', mbError, MB_OK);
    Result := False;
  end;
end;

function IsWindows10OrNewer(): Boolean;
begin
  Result := (GetWindowsVersion >= $0A000000);
end;
