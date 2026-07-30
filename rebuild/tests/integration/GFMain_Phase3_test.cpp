#include <stdio.h>
#include <wchar.h>

#include "fable_gfmain.h"
#include "fable_string.h"
#include "fable_text_layout.h"

namespace
{
    const wchar_t kExpectedPath[] =
        L"Data\\lang\\English\\lang_settings.txt";

    void ResetObservations(GFMainPhase3State& state)
    {
        state.pathExistsCalls = 0;
        state.diskFileConstructCalls = 0;
        state.loadFromFileCalls = 0;
        state.stringParserConstructCalls = 0;
        state.persistContextConstructCalls = 0;
        state.transferCalls = 0;
        state.loadImeSettingsCalls = 0;
        state.sortTreeCalls = 0;
        state.diskFileCloseCalls = 0;
        state.resolvedSettingsPath[0] = L'\0';
    }

    bool HasParsedSequence(const GFMainPhase3State& state)
    {
        return state.pathExistsCalls == 1 &&
            state.diskFileConstructCalls == 1 &&
            state.loadFromFileCalls == 1 &&
            state.stringParserConstructCalls == 1 &&
            state.persistContextConstructCalls == 1 &&
            state.transferCalls == 3 &&
            state.loadImeSettingsCalls == 1 &&
            state.sortTreeCalls == 1 &&
            state.diskFileCloseCalls == 1;
    }
}

int main()
{
    const fable_i32 wideStringBaseline =
        g_CWideStringInstanceCount_013BCA20;
    const fable_i32 charStringBaseline =
        g_CCharStringInstanceCount_013BD800;

    GFMainPhase3State state = {};
    state.settingsFileExists = true;
    state.settingsFileOpened = true;
    state.hasLeftAlignText = true;
    state.persistedLeftAlignText = 1;
    state.hasNoHangulWordWrap = true;
    state.persistedNoHangulWordWrap = 0;
    state.hasDisableCapsLock = false;
    state.persistedDisableCapsLock = 1;

    g_FableLeftAlignText_013B861B = 0;
    g_FableNoHangulWordWrap_013B861C = 1;
    g_FableDisableCapsLock_013B864F = 1;
    g_FablePrimaryLeftAlignment_013CA7EA = false;
    g_FableSecondaryLeftAlignment_013CA7EB = true;

    if (FableRunGFMainPhase3(state) != 0 ||
        wcscmp(state.resolvedSettingsPath, kExpectedPath) != 0 ||
        !HasParsedSequence(state) ||
        g_CWideStringInstanceCount_013BCA20 != wideStringBaseline ||
        g_CCharStringInstanceCount_013BD800 != charStringBaseline)
    {
        printf("FABLETLC_GFMAIN_PHASE3_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (g_FableLeftAlignText_013B861B != 1 ||
        g_FableNoHangulWordWrap_013B861C != 0 ||
        g_FableDisableCapsLock_013B864F != 0 ||
        !g_FablePrimaryLeftAlignment_013CA7EA ||
        g_FableSecondaryLeftAlignment_013CA7EB)
    {
        printf("FABLETLC_GFMAIN_PHASE3_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    ResetObservations(state);
    state.settingsFileOpened = false;
    g_FableLeftAlignText_013B861B = 0;
    g_FableNoHangulWordWrap_013B861C = 1;
    g_FableDisableCapsLock_013B864F = 1;

    if (FableRunGFMainPhase3(state) != 0 ||
        state.pathExistsCalls != 1 ||
        state.diskFileConstructCalls != 1 ||
        state.diskFileCloseCalls != 1 ||
        state.loadFromFileCalls != 0 ||
        state.transferCalls != 0 ||
        g_FableLeftAlignText_013B861B != 0 ||
        g_FableNoHangulWordWrap_013B861C != 1 ||
        g_FableDisableCapsLock_013B864F != 1 ||
        g_FablePrimaryLeftAlignment_013CA7EA ||
        !g_FableSecondaryLeftAlignment_013CA7EB ||
        g_CWideStringInstanceCount_013BCA20 != wideStringBaseline ||
        g_CCharStringInstanceCount_013BD800 != charStringBaseline)
    {
        printf("FABLETLC_GFMAIN_PHASE3_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    ResetObservations(state);
    state.settingsFileExists = false;
    g_FableLeftAlignText_013B861B = 1;
    g_FableNoHangulWordWrap_013B861C = 0;

    if (FableRunGFMainPhase3(state) != 0 ||
        wcscmp(state.resolvedSettingsPath, kExpectedPath) != 0 ||
        state.pathExistsCalls != 1 ||
        state.diskFileConstructCalls != 0 ||
        state.diskFileCloseCalls != 0 ||
        !g_FablePrimaryLeftAlignment_013CA7EA ||
        g_FableSecondaryLeftAlignment_013CA7EB ||
        g_CWideStringInstanceCount_013BCA20 != wideStringBaseline ||
        g_CCharStringInstanceCount_013BD800 != charStringBaseline)
    {
        printf("FABLETLC_GFMAIN_PHASE3_BEHAVIOR FAIL code=4\n");
        return 4;
    }

    printf("FABLETLC_GFMAIN_PHASE3_BEHAVIOR PASS\n");
    return 0;
}
