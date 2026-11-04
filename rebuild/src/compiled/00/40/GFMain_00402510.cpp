// GFMain @ 00402510. Native C++ coordinator; two register-selection bytes
// remain different from retail. gate_native_gfmain.py checks the full body,
// every relocation and execution against retail with simulated callees.
#include "fable_system.h"
#include "fable_string_parser.h"
#include "fable_persist_context.h"
#include "fable_disk_file.h"
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_resource_directories.h"
#include "fable_language.h"
#include "fable_def_string.h"
#include "fable_registry.h"
#include "fable_profile.h"
#include "fable_startup.h"
#include "fable_write_permissions.h"
#include "fable_ime.h"
#include "fable_cgame.h"
#include "fable_user_profile.h"
#include "fable_text_layout.h"
#include "fable_file_installer.h"
#include "fable_system_manager_state.h"
#include "engine/CConfigOptions.h"
#include <stdlib.h>

// Address labels retain globals whose owning declarations remain unresolved.
namespace NBankFileManager { void FABLE_FASTCALL SetHeaderDir(const CWideString&); }
namespace EULA { int FABLE_FASTCALL DoEULAThings(); }
class CXMVPlayer { public: static void FABLE_FASTCALL SetLeftAlignText(bool); };
class CMainGameComponent { public: static void FABLE_FASTCALL GenerateMetFilesFromLugFiles(); };
CSystemManager* FABLE_FASTCALL GFGetSystemManager();
bool FABLE_FASTCALL GFInitialise();
void FABLE_CDECL GFUninitialise();
bool FABLE_FASTCALL GFConfigDetection();
bool FABLE_FASTCALL GFFreeConfigDetection();
void FABLE_STDCALL GFHandleSystemInitError(unsigned long);
void FABLE_FASTCALL InitialiseConsoleVariables();
CWideString FABLE_FASTCALL GetWindowTitle();
extern void* g_013bc860;
extern void* g_013bc864;
extern bool g_0137548f, g_FableAllowDataGeneration_01375459, g_0138e188, g_0138e189, g_0138dd3b;
extern bool g_FableUseCompiledSoundSymbols_013B8619, g_013bc9f0, g_FableRunFromDVD_013B8615, g_01375447, g_013b860b;
extern bool g_013b8634, g_01375457, g_013b861b, g_013b861c, g_013b864f;
extern bool g_FableUseCompiledDefs_013B8617, g_013ca7d8;
extern unsigned char g_FableStartMainGame_013B8605;
extern bool g_013b8604, g_013b8607, g_013b8608, g_FablePlayIntroMovies_0137544A;
extern unsigned char g_01375449;
extern bool g_013b861e, g_013b861d, g_013b8648;
extern CConfigOptions GConfigOptions;
extern bool g_FablePresentationIntervalOverride_013B8628;
extern unsigned char g_01375468;
extern long g_01375464, g_0137545c, g_01375460, g_013b8624, g_01375474, g_01375478;
extern bool g_01375754;
extern CBankFileManager g_013ca79c;
extern CWideString g_013ca7dc;
extern "C" __declspec(dllimport) void* FABLE_STDCALL CreateFileW(const wchar_t*, unsigned long,
    unsigned long, void*, unsigned long, unsigned long, void*);
extern "C" __declspec(dllimport) int FABLE_STDCALL CloseHandle(void*);
extern "C" __declspec(dllimport) int FABLE_STDCALL MessageBoxW(HWND__*, const wchar_t*, const wchar_t*, unsigned int);

// The native template body must be visible to the optimizer, even when the
// Reset call stays out of line. This is the already verified 00403e40 body.
template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, (void*)Data);
}

struct HINSTANCE__;
extern "C" long FABLE_FASTCALL GFMain(HINSTANCE__* instance, char* commandLine, int show)
{
    NProfileTimer::StartProfile("Parse Command Line", 0);
    CSystemManagerInit init;
    CWideString profile;
    CAFile::m_errorCallback = &DoDirtyDisc;
    g_013bc860 = reinterpret_cast<void*>(&DoDirtyDisc);
    CAFile::m_harddiskErrorCallback = &GFHardDiskError;
    g_013bc864 = reinterpret_cast<void*>(&GFHardDiskError);
    CAFile::SetCurrentPath(CAFile::GetProjectPath());
    if (g_0137548f)
        InitialiseConsoleVariables();
    NProfileTimer::EndProfile(0);

    NProfileTimer::StartProfile("Setup Basic install files", 0);
    g_0138e188 = g_FableAllowDataGeneration_01375459;
    g_0138e189 = g_FableAllowDataGeneration_01375459;
    g_0138dd3b = g_FableAllowDataGeneration_01375459;
    g_013bc9f0 = g_FableUseCompiledSoundSymbols_013B8619;
    if (g_FableRunFromDVD_013B8615 && !g_01375447)
    {
        const bool startupFlag = g_013b860b;
        CFileInstallerSingleton::Get().basicInstallStartupFlag = startupFlag;
    }
    if (g_013b8634)
        FableClearStartupLatch_013964A8();
    CBankFileAsync::SetEnableFailureHandling(g_01375457);
    if (g_CFileInstallerSingleton_013CA818.reference)
    {
        --static_cast<CCPPointerInfo*>(g_CFileInstallerSingleton_013CA818.reference)->RefCount;
        if (static_cast<CCPPointerInfo*>(g_CFileInstallerSingleton_013CA818.reference)->RefCount == 0)
        {
            CCPPointerInfo* info = static_cast<CCPPointerInfo*>(g_CFileInstallerSingleton_013CA818.reference);
            info->DeleteFunc(info->Data);
            delete static_cast<CCPPointerInfo*>(g_CFileInstallerSingleton_013CA818.reference);
        }
    }
    g_CFileInstallerSingleton_013CA818.reference = 0;
    g_CFileInstallerSingleton_013CA818.object = 0;
    NProfileTimer::EndProfile(0);

    NProfileTimer::StartProfile("Setup Language", 0);
    CWideString settingsPath = L"Data\\lang\\" + NLocalisation::GetLanguageFolder().ToWideString() + L"\\lang_settings.txt";
    if (CAFile::PathExists(settingsPath))
    {
        CDiskFileWin32 file(settingsPath, FILE_ACCESS_READ, 2);
        if (file.IsOpen())
        {
            CCharString text;
            text.LoadFromFile(file, 0, -1);
            CStringParser parser(text, 0, CCharString(""), 0);
            CPersistContext context(parser, 0, true, 0, 0, 0);
            context.Transfer<bool>("LeftAlignText", g_013b861b, false);
            context.Transfer<bool>("NoHangulWordWrap", g_013b861c, false);
            context.Transfer<bool>("DisableCapsLock", g_013b864f, false);
            NUISystem::CIME::LoadIMESettings(context);
        }
    }
    NTextLayoutConfig::SetPrimaryLeftAlignment(g_013b861b);
    NTextLayoutConfig::SetSecondaryLeftAlignment(g_013b861c);
    NProfileTimer::EndProfile(0);

    NProfileTimer::StartProfile("Setup basic retail banks", 0);
    CBasicRetailBankManager& aliases = reinterpret_cast<CBasicRetailBankManager&>(g_013ca79c);
    aliases.SetNavigatorEnabled(GUseRetailBanks);
    aliases.AddBankAlias("GBANK_MAIN_PC", "GBANK_MAIN");
    aliases.AddBankAlias("GBANK_GUI_PC", "GBANK_GUI");
    aliases.AddBankAlias("GBANK_FRONT_END_PC", "GBANK_FRONT_END");
    aliases.AddBankAlias("PARTICLE_MAIN_PC", "PARTICLE_MAIN");
    aliases.AddBankAlias("PARTICLE_FRONTEND_PC", "PARTICLE_FRONTEND");
    CWideString definitionPath;
    if (GUseRetailBanks)
    {
        if (g_FableStartMainGame_013B8605)
        {
            CWideString suffix(L"pc\\");
            g_013ca79c.OpenRetailBank(FableGetGraphicsDirectory_0041A100() + L"graphics.big", true);
            g_013ca79c.OpenRetailBank(FableGetGraphicsDirectory_0041A100() + suffix + L"textures.big", true);
            g_013ca79c.OpenRetailBank(FableGetLanguageDirectoryB_0041A330() + L"dialogue.big", false);
            g_013ca79c.OpenRetailBank(FableGetMiscDirectoryB_0041A0C0() + suffix + L"effects.big", false);
            g_013ca79c.OpenRetailBank(FableGetLanguageDirectoryB_0041A330() + L"text.big", false);
            g_013ca79c.OpenRetailBank(FableGetMiscDirectoryA_0041A0A0() + L"temp.big", false);
            g_013ca79c.OpenRetailBank(FableGetShadersDirectory_0041A1E0() + suffix + L"shaders.big", false);
        }
        g_013ca79c.OpenRetailBank(FableGetLanguageDirectoryA_0041A120() + L"fonts.big", true);
        g_013ca79c.OpenRetailBank(FableGetLanguageDirectoryB_0041A330() + L"text.big", false);
        NBankFileManager::SetHeaderDir(L"data\\defs\\RetailHeaders\\");
        definitionPath = L"Data\\CompiledDefs\\";
    }
    else
    {
        if (g_FableRunFromDVD_013B8615)
            g_013ca79c.OpenIniFile(L"banks_dvd.ini");
        else
            g_013ca79c.OpenIniFile(L"banks.ini");
        NBankFileManager::SetHeaderDir(L"data\\defs\\DevHeaders\\");
        definitionPath = L"Data\\CompiledDefs\\Development\\";
    }
    NProfileTimer::EndProfile(0);

    NProfileTimer::StartProfile("Setup definition manager table", 0);
    g_013ca7dc = definitionPath;
    GDefStringTable.SetTableFilename(definitionPath + L"names.bin");
    g_013ca7d8 = g_FableUseCompiledDefs_013B8617;
    if (g_FableUseCompiledDefs_013B8617)
        GDefStringTable.LoadTable(!g_FableAllowDataGeneration_01375459);
    NProfileTimer::EndProfile(0);

    unsigned long permission;
    void* directory = CreateFileW(FableGetAutoSaveCheckpointDirectory_0041A4C0(), 1, 1, 0, 3, 0x02000000, 0);
    if (directory != reinterpret_cast<void*>(-1))
    {
        CloseHandle(directory);
        permission = FableCheckMyDocumentsWritePermissions_00402130(FableGetAutoSaveCheckpointDirectory_0041A4C0());
        directory = CreateFileW(NResourceDirectoryNames::GetSaveDir(), 1, 1, 0, 3, 0x02000000, 0);
        if (directory != reinterpret_cast<void*>(-1))
        {
            CloseHandle(directory);
            permission = FableCheckMyDocumentsWritePermissions_00402130(static_cast<const wchar_t*>(NResourceDirectoryNames::GetSaveDir()));
        }
    }
    else
        permission = FableCheckMyDocumentsWritePermissions_00402130(NResourceDirectoryNames::GetMyDocumentsDir());
    if (permission > 0)
    {
        CCountedPointer<NGameText::CDataBank> bank;
        bank.Reset(new NGameText::CDataBank);
        bank->OpenReadOnly(NLocalisation::GetTextBankName(), 0x100);
        CWideString message;
        unsigned long index = bank->FindIndexBySymbolCRC(permission);
        if (index > 0)
        {
            CIVCountedPointer<const NGameText::CTextEntry> entry = bank->GetPTextEntry(index, true);
            if (entry.operator->())
                message = entry->GetText();
        }
        CWideString caption;
        CCharString captionKey("TEXT_GUI_WINLOGO_ERROR_CAPTION");
        index = bank->FindIndexBySymbolCRC(captionKey.GetCRC());
        if (index > 0)
        {
            CIVCountedPointer<const NGameText::CTextEntry> entry = bank->GetPTextEntry(index, true);
            if (entry.operator->())
                caption = entry->GetText();
        }
        HWND__* window = static_cast<HWND__*>(GFGetSystemManager()->ApplicationWinHandle);
        MessageBoxW(window, message, caption, 0);
        return 1;
    }

    NProfileTimer::StartProfile("Setup library", 0);
    init.InstanceHandle = instance;
    init.CommandLine = commandLine;
    init.WindowsShow = show;
    init.Flags = 5;
    init.PreferPrimaryDisplayDevice = g_013b8604;
    if (g_01375449 == 1)
    {
        init.Flags = 0x15;
        init.SoundManagerInit.unknown08 = 32.0f;
        init.SoundManagerInit.unknown0C = 5.0f;
        init.SoundManagerInit.unknown10 = 1.0f;
        init.SoundManagerInit.unknown14 = 0x100;
        init.SoundManagerInit.unknown1C = 0x10000;
    }
    init.DisplayManagerInit.Depth = g_01375464;
    init.DisplayManagerInit.Width = g_0137545c;
    init.DisplayManagerInit.Height = g_01375460;
    init.DisplayManagerInit.ZBufferDepth = g_01375468;
    init.DisplayManagerInit.RefreshRate = g_013b8624;
    init.DisplayManagerInit.PresentImmediate = g_FablePresentationIntervalOverride_013B8628;
    init.DisplayManagerInit.PushBufferSize = g_01375474;
    init.DisplayManagerInit.KickOffSize = g_01375478;
    init.DisplayManagerInit.ShaderResourcePath = FableGetShadersDirectory_0041A1E0();
    if (g_013b8607)
        init.DisplayManagerInit.MultisampleType = g_013b8608 ? 9 : 4;
    init.DisplayManagerInit.UseRenderManager = true;
    init.DisplayManagerInit.NoBackBuffers = 1;
    init.IsErrorFailureFunc = &GFIsSystemErrorFailure;
    init.AppName = GetWindowTitle();
    init.ExclusiveMode = g_FablePlayIntroMovies_0137544A;
    init.TempPath = FableGetCacheDirectory_0041A180();
    init.StreamingFontBankHandle = FableGetStreamingFontBankName_00415440();
    init.WaitWhileInactive = !g_013b861e;
    init.InputManagerInit.UseJoystick = true;
    init.InputManagerInit.UseKeyboard = true;
    init.InputManagerInit.UseMouse = true;
    init.SkipConfigDetection = g_013b861d;
    if (g_013b8648)
    {
        init.SkipConfigDetection = true;
        g_01375754 = 0;
    }
    else
    {
        if (!EULA::DoEULAThings())
            exit(1);
        if (!init.SkipConfigDetection && !g_013b8648)
        {
            if (!GFConfigDetection())
                exit(1);
            GFFreeConfigDetection();
        }
    }
    if (GConfigOptions.UseAlternateRenderTargetClear)
        init.DisplayManagerInit.RenderManagerInitFlags |= 1;
    if (!GConfigOptions.EnableTextureTransforms)
        init.DisplayManagerInit.RenderManagerInitFlags |= 2;
    if (GConfigOptions.UseFixedFunctionText)
        CXMVPlayer::SetLeftAlignText(true);
    init.IMEMessageFunc = &NUISystem::CIME::ProcessWinMessage;
    init.MessageFilter = &NUISystem::CIME::FilterMessage;
    ESystemCode code = GFGetSystemManager()->Initialise(&init);
    NProfileTimer::EndProfile(0);
    NUISystem::CIME::Get().Initialise(static_cast<HWND__*>(GFGetSystemManager()->ApplicationWinHandle));
    NProfileTimer::StartProfile("End basic init", 0);
    if (g_FableAllowDataGeneration_01375459)
        CMainGameComponent::GenerateMetFilesFromLugFiles();
    if (code == kSystemCodeSuccess)
    {
        if (GFInitialise())
        {
            if (profile != L"")
                CUserProfileManager::Get().SetProfileName(profile, true);
            NProfileTimer::EndProfile(0);
            g_Game_013B83D0.Play();
            GFUninitialise();
        }
    }
    else
        GFHandleSystemInitError(code);
    NProfileTimer::EndProfile(0);
    {
        CSystemRegistry registry(CSystemRegistry::GAME_REG_KEY, 0);
        registry.WriteBOOL(CSystemRegistry::GFX_RESET_KEY_NAME, 0);
    }
    return 0;
}
