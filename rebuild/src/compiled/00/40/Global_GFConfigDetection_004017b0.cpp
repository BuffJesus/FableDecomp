#include <string.h>

#include "fable_config_detection.h"
#include "fable_registry.h"
#include "engine/CConfigOptions.h"
#include "engine/ConfigDetection.h"

// GFConfigDetection (fablelib/main.cpp): loads ConfigDetect.dll, lets it
// probe the hardware into pConfig, and turns its verdicts into the engine's
// CConfigOptions (detail-slider ceilings/defaults, feature switches, the
// safe-mode software fallback remembered through the registry).

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

typedef int (__stdcall* DISPLAYERRORDIALOG)(int*, int);
typedef int (__stdcall* CREATECONFIGDETECTION)(
    ConfigDetection**,
    DISPLAYERRORDIALOG);
typedef int (__stdcall* CONFIG_INITVIDEOCARDINFO)(HWND__*, char*);
typedef void (__stdcall* CHECKREQUIREMENTS)();

bool FABLE_FASTCALL GFConfigDetection()
{
    InitExternalStrings();

    g_FableConfigDetectionModule_013B7C8C = LoadLibraryA("ConfigDetect.dll");
    if (g_FableConfigDetectionModule_013B7C8C == 0)
        return false;

    CREATECONFIGDETECTION pCreateConfigDetection = 0;
    pCreateConfigDetection = (CREATECONFIGDETECTION)GetProcAddress(
        (HMODULE)g_FableConfigDetectionModule_013B7C8C,
        "CreateConfigDetection");
    if (pCreateConfigDetection == 0)
        return false;
    if (!pCreateConfigDetection(&pConfig, DisplayErrorDialog))
        return false;

    CONFIG_INITVIDEOCARDINFO pConfig_InitVideoCardInfo = 0;
    pConfig_InitVideoCardInfo = (CONFIG_INITVIDEOCARDINFO)GetProcAddress(
        (HMODULE)g_FableConfigDetectionModule_013B7C8C,
        "Config_InitVideoCardInfo");
    if (pConfig_InitVideoCardInfo == 0)
        return false;

    char szDebug[260];
    strncpy(szDebug, "unspecified", 259);
    if (pConfig_InitVideoCardInfo(0, szDebug))
    {
        CHECKREQUIREMENTS pCheckRequirements = 0;
        pCheckRequirements = (CHECKREQUIREMENTS)GetProcAddress(
            (HMODULE)g_FableConfigDetectionModule_013B7C8C,
            "CheckRequirements");
        if (pCheckRequirements != 0)
            pCheckRequirements();
    }
    else
    {
        MessageBoxA(0, szDebug, "ConfigDetection Error", 0);
        return false;
    }

    GConfigOptions.UseAlternateRenderTargetClear =
        pConfig->cf_UseAlternateRenderTargetClear != 0;
    GConfigOptions.UseAlternateOutlinePixelShader =
        pConfig->cf_UseAlternateOutlineEffect != 0;
    GConfigOptions.EnableStretchBlitEffects =
        pConfig->cf_DisableStretchRect == 0;
    GConfigOptions.UseAlternateSkyPixelShader =
        pConfig->cf_UseAlternateSkyPixelShader != 0;
    GConfigOptions.EnableFilteredScreenGUIBackground =
        pConfig->cf_DisableFilteredScreenGUIBackground == 0;
    GConfigOptions.EnableTextureTransforms =
        pConfig->cf_DisableTextureTransforms == 0;
    GConfigOptions.UseFixedFunctionText = pConfig->cf_UseFixedFunction != 0;

    GConfigOptions.EffectDetailSliderMax = pConfig->cf_MaxEffectsDetail - 1;
    GConfigOptions.ShadowDetailSliderMax = pConfig->cf_MaxShadowDetail - 1;
    GConfigOptions.MeshDetailSliderMax = pConfig->cf_MaxMeshDetail - 1;
    GConfigOptions.TextureDetailSliderMax = pConfig->cf_MaxTextureDetail - 1;
    GConfigOptions.EffectDetailSliderDefault =
        pConfig->cf_DefaultEffectsDetail - 1;
    GConfigOptions.ShadowDetailSliderDefault =
        pConfig->cf_DefaultShadowDetail - 1;
    GConfigOptions.MeshDetailSliderDefault = pConfig->cf_DefaultMeshDetail - 1;
    GConfigOptions.TextureDetailSliderDefault =
        pConfig->cf_DefaultTextureDetail - 1;
    GConfigOptions.MaxAntiAliasLevel = pConfig->cf_MaxAntiAliasLevel - 1;

    if (pConfig->cf_MaximumResolution > 0)
        GConfigOptions.MaxScreenResolutionWidth = pConfig->cf_MaximumResolution;
    if (pConfig->cf_GenericFlag0 > 0)
        GConfigOptions.ScreenResolutionWidthDefault = pConfig->cf_GenericFlag0;
    if (pConfig->cf_GenericFlag1 > 0)
        GConfigOptions.ScreenResolutionHeightDefault = pConfig->cf_GenericFlag1;

    CSystemRegistry reg(CSystemRegistry::GAME_REG_KEY, 0);
    int gfx_reset = reg.GetBOOL(CSystemRegistry::GFX_RESET_KEY_NAME, 0);

    GConfigOptions.SafeModeSelected = pConfig->cf_SafeMode != 0;
    if (!GConfigOptions.SafeModeSelected && gfx_reset)
    {
        int Error[2];
        Error[0] = 0x801;
        Error[1] = 0;
        if (DisplayErrorDialog(Error, 0) == 1)
            GConfigOptions.SafeModeSelected = true;
    }
    reg.WriteBOOL(CSystemRegistry::GFX_RESET_KEY_NAME, 1);

    if (GConfigOptions.SafeModeSelected)
    {
        GConfigOptions.EffectDetailSliderMax = 0;
        GConfigOptions.ShadowDetailSliderMax = 0;
        GConfigOptions.MeshDetailSliderMax = 0;
        GConfigOptions.TextureDetailSliderMax = 0;
        GConfigOptions.EffectDetailSliderDefault = 0;
        GConfigOptions.ShadowDetailSliderDefault = 0;
        GConfigOptions.MeshDetailSliderDefault = 0;
        GConfigOptions.TextureDetailSliderDefault = 0;
        GConfigOptions.MaxAntiAliasLevel = 0;
        GConfigOptions.MaxScreenResolutionWidth = 1024;
        GConfigOptions.MaxRefreshRate = 60;
    }

    if (GConfigOptions.EffectDetailSliderMax >= 0 &&
        GConfigOptions.EffectDetailSliderDefault >= 0 &&
        GConfigOptions.EffectDetailSliderMax <
            GConfigOptions.EffectDetailSliderDefault)
        GConfigOptions.EffectDetailSliderDefault =
            GConfigOptions.EffectDetailSliderMax;
    if (GConfigOptions.ShadowDetailSliderMax >= 0 &&
        GConfigOptions.ShadowDetailSliderDefault >= 0 &&
        GConfigOptions.ShadowDetailSliderMax <
            GConfigOptions.ShadowDetailSliderDefault)
        GConfigOptions.ShadowDetailSliderDefault =
            GConfigOptions.ShadowDetailSliderMax;
    if (GConfigOptions.MeshDetailSliderMax >= 0 &&
        GConfigOptions.MeshDetailSliderDefault >= 0 &&
        GConfigOptions.MeshDetailSliderMax <
            GConfigOptions.MeshDetailSliderDefault)
        GConfigOptions.MeshDetailSliderDefault =
            GConfigOptions.MeshDetailSliderMax;
    if (GConfigOptions.TextureDetailSliderMax >= 0 &&
        GConfigOptions.TextureDetailSliderDefault >= 0 &&
        GConfigOptions.TextureDetailSliderMax <
            GConfigOptions.TextureDetailSliderDefault)
        GConfigOptions.TextureDetailSliderDefault =
            GConfigOptions.TextureDetailSliderMax;

    return true;
}
