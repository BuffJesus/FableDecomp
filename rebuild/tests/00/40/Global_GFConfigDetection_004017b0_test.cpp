#include <stdio.h>
#include <string.h>

#include "fable_config_detection.h"
#include "fable_registry.h"
#include "engine/CConfigOptions.h"
#include "engine/ConfigDetection.h"

// GFConfigDetection reaches kernel32/user32 through the import table. The
// fixture redirects its own LoadLibraryA/GetProcAddress slots to recorders
// (forwarding every call that is not ConfigDetect.dll's); the toolchain has
// no user32.lib, so MessageBoxA's import slot is a fixture variable bound by
// /alternatename. It also
// defines the real callees -- InitExternalStrings, DisplayErrorDialog, the
// CSystemRegistry members and statics, pConfig, GConfigOptions -- as
// recorders.

#pragma comment(linker, "/alternatename:__imp__MessageBoxA@16=_g_FixtureMessageBoxASlot")

struct HINSTANCE__;
typedef HINSTANCE__* HMODULE;
struct HWND__;
typedef int (__stdcall* FARPROC)();

extern "C" __declspec(dllimport) HMODULE __stdcall LoadLibraryA(
    const char* fileName);
extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(
    HMODULE module,
    const char* procName);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(
    HWND__* window,
    const char* text,
    const char* caption,
    unsigned int type);
extern "C" __declspec(dllimport) int __stdcall VirtualProtect(
    void* address,
    unsigned long size,
    unsigned long protect,
    unsigned long* oldProtect);

extern "C" char __ImageBase;

void* g_FableConfigDetectionModule_013B7C8C = 0;
ConfigDetection* pConfig = 0;
CConfigOptions GConfigOptions;

// CWideString is only needed for the two registry statics' addresses.
CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}
const CWideString CSystemRegistry::GAME_REG_KEY;
const CWideString CSystemRegistry::GFX_RESET_KEY_NAME;

namespace
{
    typedef HMODULE (__stdcall* LoadLibraryAFunction)(const char*);
    typedef FARPROC (__stdcall* GetProcAddressFunction)(HMODULE, const char*);
    typedef int (__stdcall* MessageBoxAFunction)(
        HWND__*,
        const char*,
        const char*,
        unsigned int);

    LoadLibraryAFunction g_realLoadLibraryA;
    GetProcAddressFunction g_realGetProcAddress;

    HMODULE const kConfigModule = reinterpret_cast<HMODULE>(0x11112222);

    ConfigDetection g_info;
    bool g_loadSucceeds;
    bool g_createPresent;
    bool g_createSucceeds;
    bool g_videoPresent;
    bool g_videoSucceeds;
    bool g_requirementsPresent;
    int g_registryRead;
    int g_dialogResult;
    fable_u32 g_initStringsCalls;
    fable_u32 g_loadCalls;
    fable_u32 g_getProcCalls;
    fable_u32 g_createCalls;
    fable_u32 g_videoCalls;
    fable_u32 g_requirementsCalls;
    fable_u32 g_messageCalls;
    fable_u32 g_registryConstructCalls;
    fable_u32 g_registryReadCalls;
    fable_u32 g_registryWriteCalls;
    fable_u32 g_registryDestroyCalls;
    fable_u32 g_dialogCalls;
    bool g_argumentsOk;
    int g_observedDialogCode;
    int g_observedDialogExtra;
    int g_observedWrite;
    const void* g_registryThis;
    char g_observedMessage[64];

    void Reset()
    {
        memset(&g_info, 0, sizeof(g_info));
        pConfig = 0;
        g_FableConfigDetectionModule_013B7C8C = 0;
        g_loadSucceeds = true;
        g_createPresent = true;
        g_createSucceeds = true;
        g_videoPresent = true;
        g_videoSucceeds = true;
        g_requirementsPresent = true;
        g_registryRead = 0;
        g_dialogResult = 0;
        g_initStringsCalls = 0;
        g_loadCalls = 0;
        g_getProcCalls = 0;
        g_createCalls = 0;
        g_videoCalls = 0;
        g_requirementsCalls = 0;
        g_messageCalls = 0;
        g_registryConstructCalls = 0;
        g_registryReadCalls = 0;
        g_registryWriteCalls = 0;
        g_registryDestroyCalls = 0;
        g_dialogCalls = 0;
        g_argumentsOk = true;
        g_observedDialogCode = 0;
        g_observedDialogExtra = -1;
        g_observedWrite = -1;
        g_registryThis = 0;
        g_observedMessage[0] = '\0';

        memset(&GConfigOptions, 0xCD, sizeof(GConfigOptions));
        GConfigOptions.MaxScreenResolutionWidth = 640;
        GConfigOptions.MaxRefreshRate = 30;
        GConfigOptions.ScreenResolutionWidthDefault = 1;
        GConfigOptions.ScreenResolutionHeightDefault = 2;

        g_info.cf_MaximumResolution = 800;
        g_info.cf_UseFixedFunction = 1;
        g_info.cf_UseAlternateRenderTargetClear = 1;
        g_info.cf_UseAlternateOutlineEffect = 0;
        g_info.cf_DisableStretchRect = 0;
        g_info.cf_UseAlternateSkyPixelShader = 1;
        g_info.cf_DisableFilteredScreenGUIBackground = 0;
        g_info.cf_DisableTextureTransforms = 1;
        g_info.cf_MaxEffectsDetail = 3;
        g_info.cf_MaxShadowDetail = 8;
        g_info.cf_MaxMeshDetail = 2;
        g_info.cf_MaxTextureDetail = 1;
        g_info.cf_DefaultEffectsDetail = 6;
        g_info.cf_DefaultShadowDetail = 3;
        g_info.cf_DefaultMeshDetail = 5;
        g_info.cf_DefaultTextureDetail = 3;
        g_info.cf_MaxAntiAliasLevel = 9;
        g_info.cf_GenericFlag0 = 128;
        g_info.cf_GenericFlag1 = 256;
    }

    int __stdcall CreateConfigDetection(
        ConfigDetection** output,
        int (__stdcall* errorCallback)(int*, int))
    {
        ++g_createCalls;
        if (output != &pConfig || errorCallback != &DisplayErrorDialog)
            g_argumentsOk = false;
        *output = &g_info;
        return g_createSucceeds ? 1 : 0;
    }

    int __stdcall InitialiseVideoCard(HWND__* window, char* error)
    {
        ++g_videoCalls;
        if (window != 0 || strcmp(error, "unspecified") != 0)
            g_argumentsOk = false;
        if (!g_videoSucceeds)
            strcpy(error, "video failed");
        return g_videoSucceeds ? 1 : 0;
    }

    void __stdcall CheckRequirements()
    {
        ++g_requirementsCalls;
    }

    HMODULE __stdcall TestLoadLibraryA(const char* name)
    {
        if (strcmp(name, "ConfigDetect.dll") != 0)
            return g_realLoadLibraryA(name);
        ++g_loadCalls;
        return g_loadSucceeds ? kConfigModule : 0;
    }

    FARPROC __stdcall TestGetProcAddress(HMODULE module, const char* name)
    {
        if (module != kConfigModule)
            return g_realGetProcAddress(module, name);
        ++g_getProcCalls;
        if (strcmp(name, "CreateConfigDetection") == 0)
            return g_createPresent
                ? reinterpret_cast<FARPROC>(&CreateConfigDetection)
                : 0;
        if (strcmp(name, "Config_InitVideoCardInfo") == 0)
            return g_videoPresent
                ? reinterpret_cast<FARPROC>(&InitialiseVideoCard)
                : 0;
        if (strcmp(name, "CheckRequirements") == 0)
            return g_requirementsPresent
                ? reinterpret_cast<FARPROC>(&CheckRequirements)
                : 0;
        return 0;
    }

    int __stdcall TestMessageBoxA(
        HWND__* window,
        const char* text,
        const char* caption,
        unsigned int type)
    {
        ++g_messageCalls;
        if (window != 0 || strcmp(caption, "ConfigDetection Error") != 0 ||
            type != 0)
            g_argumentsOk = false;
        strncpy(g_observedMessage, text, sizeof(g_observedMessage) - 1);
        g_observedMessage[sizeof(g_observedMessage) - 1] = '\0';
        return 1;
    }

    void** FindImport(const char* name)
    {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<fable_u32*>(base + 0x3C);
        fable_u32 rva = *reinterpret_cast<fable_u32*>(nt + 0x80);
        for (fable_u32* entry = reinterpret_cast<fable_u32*>(base + rva);
             entry[3] != 0;
             entry += 5)
        {
            fable_u32* names = reinterpret_cast<fable_u32*>(
                base + (entry[0] ? entry[0] : entry[4]));
            void** slots = reinterpret_cast<void**>(base + entry[4]);
            for (int i = 0; names[i] != 0; ++i)
            {
                if ((names[i] & 0x80000000u) == 0 &&
                    strcmp(base + names[i] + 2, name) == 0)
                    return &slots[i];
            }
        }
        return 0;
    }

    bool Patch(const char* name, void* replacement, void** original)
    {
        void** slot = FindImport(name);
        unsigned long oldProtect = 0;
        if (slot == 0 || !VirtualProtect(slot, sizeof(void*), 0x04, &oldProtect))
            return false;
        *original = *slot;
        *slot = replacement;
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        return true;
    }

    bool InstallImports()
    {
        return
            Patch(
                "LoadLibraryA",
                reinterpret_cast<void*>(&TestLoadLibraryA),
                reinterpret_cast<void**>(&g_realLoadLibraryA)) &&
            Patch(
                "GetProcAddress",
                reinterpret_cast<void*>(&TestGetProcAddress),
                reinterpret_cast<void**>(&g_realGetProcAddress));
    }

    bool NoRegistry()
    {
        return g_registryConstructCalls == 0 &&
            g_registryReadCalls == 0 &&
            g_registryWriteCalls == 0 &&
            g_registryDestroyCalls == 0;
    }

    bool CheckLoadFailure()
    {
        Reset();
        g_loadSucceeds = false;
        return !GFConfigDetection() &&
            g_initStringsCalls == 1 &&
            g_loadCalls == 1 &&
            g_getProcCalls == 0 &&
            g_FableConfigDetectionModule_013B7C8C == 0 &&
            NoRegistry();
    }

    bool CheckMissingCreate()
    {
        Reset();
        g_createPresent = false;
        return !GFConfigDetection() &&
            g_FableConfigDetectionModule_013B7C8C == kConfigModule &&
            g_getProcCalls == 1 &&
            g_createCalls == 0 &&
            NoRegistry();
    }

    bool CheckCreateFailure()
    {
        Reset();
        g_createSucceeds = false;
        return !GFConfigDetection() &&
            g_createCalls == 1 &&
            g_getProcCalls == 1 &&
            g_videoCalls == 0 &&
            g_argumentsOk &&
            NoRegistry();
    }

    bool CheckMissingVideo()
    {
        Reset();
        g_videoPresent = false;
        return !GFConfigDetection() &&
            g_createCalls == 1 &&
            g_getProcCalls == 2 &&
            g_videoCalls == 0 &&
            g_messageCalls == 0 &&
            NoRegistry();
    }

    bool CheckVideoFailure()
    {
        Reset();
        g_videoSucceeds = false;
        return !GFConfigDetection() &&
            g_videoCalls == 1 &&
            g_messageCalls == 1 &&
            g_getProcCalls == 2 &&
            g_requirementsCalls == 0 &&
            strcmp(g_observedMessage, "video failed") == 0 &&
            g_argumentsOk &&
            NoRegistry();
    }

    bool CheckNormalSuccessAndClamping()
    {
        Reset();
        const bool result = GFConfigDetection();
        const CConfigOptions& o = GConfigOptions;
        return result &&
            g_initStringsCalls == 1 &&
            g_getProcCalls == 3 &&
            g_requirementsCalls == 1 &&
            g_messageCalls == 0 &&
            g_argumentsOk &&
            o.UseAlternateRenderTargetClear == true &&
            o.UseAlternateOutlinePixelShader == false &&
            o.EnableStretchBlitEffects == true &&
            o.UseAlternateSkyPixelShader == true &&
            o.EnableFilteredScreenGUIBackground == true &&
            o.EnableTextureTransforms == false &&
            o.UseFixedFunctionText == true &&
            o.SafeModeSelected == false &&
            o.EffectDetailSliderMax == 2 &&
            o.EffectDetailSliderDefault == 2 &&
            o.ShadowDetailSliderMax == 7 &&
            o.ShadowDetailSliderDefault == 2 &&
            o.MeshDetailSliderMax == 1 &&
            o.MeshDetailSliderDefault == 1 &&
            o.TextureDetailSliderMax == 0 &&
            o.TextureDetailSliderDefault == 0 &&
            o.MaxAntiAliasLevel == 8 &&
            o.MaxScreenResolutionWidth == 800 &&
            o.MaxRefreshRate == 30 &&
            o.ScreenResolutionWidthDefault == 128 &&
            o.ScreenResolutionHeightDefault == 256 &&
            g_registryConstructCalls == 1 &&
            g_registryReadCalls == 1 &&
            g_registryWriteCalls == 1 &&
            g_registryDestroyCalls == 1 &&
            g_observedWrite == 1 &&
            g_dialogCalls == 0;
    }

    bool CheckZeroResolutionsKeepDefaults()
    {
        Reset();
        g_info.cf_MaximumResolution = 0;
        g_info.cf_GenericFlag0 = 0;
        g_info.cf_GenericFlag1 = 0;
        g_info.cf_MaxShadowDetail = 0;
        const bool result = GFConfigDetection();
        const CConfigOptions& o = GConfigOptions;
        return result &&
            o.MaxScreenResolutionWidth == 640 &&
            o.ScreenResolutionWidthDefault == 1 &&
            o.ScreenResolutionHeightDefault == 2 &&
            o.ShadowDetailSliderMax == -1 &&
            o.ShadowDetailSliderDefault == 2;
    }

    bool CheckPromptAcceptedFallback()
    {
        Reset();
        g_registryRead = 1;
        g_dialogResult = 1;
        const bool result = GFConfigDetection();
        const CConfigOptions& o = GConfigOptions;
        return result &&
            g_dialogCalls == 1 &&
            g_observedDialogCode == 0x801 &&
            g_observedDialogExtra == 0 &&
            g_argumentsOk &&
            o.SafeModeSelected == true &&
            o.EffectDetailSliderMax == 0 &&
            o.ShadowDetailSliderMax == 0 &&
            o.MeshDetailSliderMax == 0 &&
            o.TextureDetailSliderMax == 0 &&
            o.EffectDetailSliderDefault == 0 &&
            o.ShadowDetailSliderDefault == 0 &&
            o.MeshDetailSliderDefault == 0 &&
            o.TextureDetailSliderDefault == 0 &&
            o.MaxAntiAliasLevel == 0 &&
            o.MaxScreenResolutionWidth == 1024 &&
            o.MaxRefreshRate == 60 &&
            g_observedWrite == 1 &&
            g_registryDestroyCalls == 1;
    }

    bool CheckPromptDeclined()
    {
        Reset();
        g_registryRead = 1;
        g_dialogResult = 2;
        const bool result = GFConfigDetection();
        return result &&
            g_dialogCalls == 1 &&
            GConfigOptions.SafeModeSelected == false &&
            GConfigOptions.MaxRefreshRate == 30 &&
            g_registryDestroyCalls == 1;
    }

    bool CheckDetectedSafeMode()
    {
        Reset();
        g_info.cf_SafeMode = 1;
        g_registryRead = 1;
        const bool result = GFConfigDetection();
        return result &&
            g_dialogCalls == 0 &&
            GConfigOptions.SafeModeSelected == true &&
            GConfigOptions.MaxScreenResolutionWidth == 1024 &&
            GConfigOptions.MaxRefreshRate == 60 &&
            g_registryDestroyCalls == 1;
    }
}

extern "C" MessageBoxAFunction g_FixtureMessageBoxASlot = &TestMessageBoxA;

extern "C" void __fastcall InitExternalStrings()
{
    ++g_initStringsCalls;
}

extern "C" int __stdcall DisplayErrorDialog(int* Error, int ForceExit)
{
    ++g_dialogCalls;
    g_observedDialogCode = Error[0];
    g_observedDialogExtra = Error[1];
    if (ForceExit != 0)
        g_argumentsOk = false;
    return g_dialogResult;
}

CSystemRegistry::CSystemRegistry(const CWideString& subKey, long rootSelection)
{
    ++g_registryConstructCalls;
    g_registryThis = this;
    if (&subKey != &GAME_REG_KEY || rootSelection != 0)
        g_argumentsOk = false;
}

CSystemRegistry::~CSystemRegistry()
{
    ++g_registryDestroyCalls;
    if (this != g_registryThis)
        g_argumentsOk = false;
}

int CSystemRegistry::GetBOOL(const CWideString& valueName, int defaultValue) const
{
    ++g_registryReadCalls;
    if (this != g_registryThis || &valueName != &GFX_RESET_KEY_NAME ||
        defaultValue != 0)
        g_argumentsOk = false;
    return g_registryRead;
}

void CSystemRegistry::WriteBOOL(const CWideString& valueName, int value) const
{
    ++g_registryWriteCalls;
    if (this != g_registryThis || &valueName != &GFX_RESET_KEY_NAME ||
        g_registryReadCalls != 1)
        g_argumentsOk = false;
    g_observedWrite = value;
}

int main()
{
    if (!InstallImports())
    {
        printf("FABLETLC_CONFIG_DETECTION_BEHAVIOR FAIL imports\n");
        return 1;
    }
    const bool load = CheckLoadFailure();
    const bool createMissing = CheckMissingCreate();
    const bool createFailure = CheckCreateFailure();
    const bool videoMissing = CheckMissingVideo();
    const bool videoFailure = CheckVideoFailure();
    const bool normal = CheckNormalSuccessAndClamping();
    const bool zero = CheckZeroResolutionsKeepDefaults();
    const bool prompt = CheckPromptAcceptedFallback();
    const bool declined = CheckPromptDeclined();
    const bool safe = CheckDetectedSafeMode();
    const bool passed =
        load && createMissing && createFailure && videoMissing &&
        videoFailure && normal && zero && prompt && declined && safe &&
        g_argumentsOk;
    if (!passed)
    {
        printf(
            "FABLETLC_CONFIG_DETECTION_BEHAVIOR FAIL "
            "load=%u cm=%u cf=%u vm=%u vf=%u normal=%u zero=%u prompt=%u "
            "declined=%u safe=%u args=%u\n",
            load,
            createMissing,
            createFailure,
            videoMissing,
            videoFailure,
            normal,
            zero,
            prompt,
            declined,
            safe,
            g_argumentsOk);
        return 1;
    }
    printf("FABLETLC_CONFIG_DETECTION_BEHAVIOR PASS\n");
    return 0;
}
