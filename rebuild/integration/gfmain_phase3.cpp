#include "fable_gfmain.h"
#include "fable_language.h"
#include "fable_profile.h"
#include "fable_string.h"
#include "fable_text_layout.h"

namespace
{
    const char kSetupLanguageProfile[] = "Setup Language";
    const wchar_t kLanguageDirectory[] = L"Data\\lang\\";
    const wchar_t kLanguageSettingsFile[] = L"\\lang_settings.txt";
}

long FABLE_FASTCALL FableRunGFMainPhase3(GFMainPhase3State& state)
{
    NProfileTimer::EndProfile(0);

    {
        CCharString profile(kSetupLanguageProfile, -1);
        NProfileTimer::StartProfile(profile, 0);
    }

    CCharString defaultLanguage =
        FableGetDefaultLanguageName_00415530();
    CWideString language = defaultLanguage.ToWideString();
    CWideString languageDirectory = kLanguageDirectory + language;
    CWideString settingsPath =
        languageDirectory + kLanguageSettingsFile;

    const wchar_t* pathname = settingsPath;
    if (FableGFMainPhase3PathExistsBoundary(pathname, state))
        FableGFMainPhase3LoadSettingsBoundary(pathname, state);

    NTextLayoutConfig::SetPrimaryLeftAlignment(
        g_FableLeftAlignText_013B861B != 0);
    NTextLayoutConfig::SetSecondaryLeftAlignment(
        g_FableNoHangulWordWrap_013B861C != 0);

    NProfileTimer::EndProfile(0);
    return 0;
}
