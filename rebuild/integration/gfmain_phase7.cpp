#include <string.h>
#include <wchar.h>

#include "fable_gfmain.h"
#include "fable_language.h"
#include "fable_profile.h"
#include "fable_resource_directories.h"
#include "fable_system.h"

namespace
{
    void CopyWide(
        wchar_t* destination,
        size_t capacity,
        const wchar_t* source)
    {
        wcsncpy(destination, source != 0 ? source : L"", capacity - 1);
        destination[capacity - 1] = L'\0';
    }

    void CopyNarrow(
        char* destination,
        size_t capacity,
        const char* source)
    {
        strncpy(destination, source != 0 ? source : "", capacity - 1);
        destination[capacity - 1] = '\0';
    }

    const char* GetText(const CCharString& value)
    {
        const CCharStringData* storage =
            *reinterpret_cast<CCharStringData* const*>(&value);
        return storage != 0 && storage->text != 0
            ? storage->text
            : "";
    }
}

long FABLE_FASTCALL FableRunGFMainPhase7(
    GFMainPhase7State& state)
{
    CCharString profile("Setup library", -1);
    NProfileTimer::StartProfile(profile, 0);
    g_FablePhase7CurrentLanguage = state.language;

    state.configuredInstanceHandleToken = state.instanceHandleToken;
    CopyNarrow(
        state.configuredCommandLine,
        sizeof(state.configuredCommandLine),
        state.commandLine);
    state.configuredWindowsShow = state.windowsShow;
    state.configuredPreferPrimaryDisplayDevice =
        state.preferPrimaryDisplayDevice;

    state.startupFlags = 5;
    state.displayDepth = 64.0f;
    state.nearPlane = 2.0f;
    state.displayScale = 1.0f;
    state.displayUnknown4C = 0;
    state.memoryBudget = 0x00040000UL;
    if (state.useAlternateDisplayDefaults)
    {
        state.startupFlags = 0x15;
        state.displayDepth = 32.0f;
        state.nearPlane = 5.0f;
        state.displayScale = 1.0f;
        state.displayUnknown4C = 0x100;
        state.memoryBudget = 0x00010000UL;
    }

    for (int index = 0; index != 4; ++index)
        state.configuredRegionWords[index] = state.regionWords[index];
    state.configuredRegionFlag18 = state.regionFlag18;
    state.configuredRegionWord20 = state.regionWord20;
    state.configuredRegionWord24 = state.regionWord24;
    state.configuredRegionFlag28 = state.regionFlag28;

    CWideString shaderSource =
        FableGetShadersDirectory_0041A1E0();
    CWideString shaderResourcePath;
    shaderResourcePath = shaderSource;
    CopyWide(
        state.shaderResourcePath,
        sizeof(state.shaderResourcePath) /
            sizeof(state.shaderResourcePath[0]),
        shaderResourcePath);

    if (state.overrideRegionMode)
    {
        state.configuredRegionMode =
            state.alternateRegionMode ? 9 : 4;
    }

    state.configuredRegionFlag10 = true;
    state.configuredRegionFlag19 = true;
    state.errorCallbackInstalled = true;

    CWideString windowTitleSource =
        FableGFMainPhase7GetWindowTitleBoundary(state);
    CWideString windowTitle;
    windowTitle = windowTitleSource;
    CopyWide(
        state.windowTitle,
        sizeof(state.windowTitle) / sizeof(state.windowTitle[0]),
        windowTitle);
    state.configuredExclusiveMode = state.exclusiveMode;

    CWideString cacheSource =
        FableGetCacheDirectory_0041A180();
    CWideString cacheDirectory;
    cacheDirectory = cacheSource;
    CopyWide(
        state.cacheDirectory,
        sizeof(state.cacheDirectory) /
            sizeof(state.cacheDirectory[0]),
        cacheDirectory);

    CCharString streamingFontSource =
        FableGetStreamingFontBankName_00415440();
    CCharString streamingFontBank;
    streamingFontBank = streamingFontSource;
    CopyNarrow(
        state.streamingFontBank,
        sizeof(state.streamingFontBank),
        GetText(streamingFontBank));

    state.configuredWaitWhileInactive =
        !state.disableInactiveWait;
    state.configuredSkipConfigDetection =
        state.skipConfigDetection;
    state.configuredUnknown58 = true;
    state.configuredUnknown59 = true;
    state.configuredUnknown5A = true;

    state.skipPhase8 = state.staticMapMode;
    if (state.staticMapMode)
    {
        state.configuredSkipConfigDetection = true;
        state.staticMapWorkEnabled = false;
    }

    CSystemManagerInit* systemInit =
        FableGetSystemManagerInitBoundary();
    *reinterpret_cast<fable_u32*>(&systemInit->InstanceHandle) =
        state.configuredInstanceHandleToken;
    systemInit->Flags = state.startupFlags;
    systemInit->AttachToExternalWindow = state.configuredRegionFlag19;
    systemInit->PreventExclusiveModeChanges = state.configuredRegionFlag10;
    systemInit->UseAccurateTimer = state.configuredExclusiveMode;
    systemInit->WaitWhileInactive =
        state.configuredPreferPrimaryDisplayDevice;
    systemInit->SoundManagerInit.unknown08 = state.displayDepth;
    systemInit->SoundManagerInit.unknown0C = state.nearPlane;
    systemInit->SoundManagerInit.unknown10 = state.displayScale;
    systemInit->SoundManagerInit.unknown14 = state.displayUnknown4C;
    systemInit->SoundManagerInit.unknown1C = state.memoryBudget;
    systemInit->InputManagerInit.UseMouse = state.configuredUnknown58;
    systemInit->InputManagerInit.UseJoystick = state.configuredUnknown59;
    systemInit->InputManagerInit.UseKeyboard = state.configuredUnknown5A;
    systemInit->IMEMessageFunc =
        reinterpret_cast<FableIMEMessageFunction>(state.configuredRegionWord20);
    systemInit->MessageFilter =
        reinterpret_cast<FableWindowMessageFilter>(state.configuredRegionWord24);

    return 0;
}
