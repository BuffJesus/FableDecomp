#include <wchar.h>

#include "fable_gfmain.h"
#include "fable_resource_directories.h"
#include "fable_string.h"

namespace
{
    void OpenBank(
        const CWideString& directory,
        const wchar_t* filename,
        bool startupPriority,
        GFMainPhase5State& state)
    {
        CWideString pathname = directory + filename;
        FableGFMainPhase5OpenRetailBankBoundary(
            pathname,
            startupPriority,
            state);
    }

    void OpenNestedBank(
        CWideString directory,
        const wchar_t* childDirectory,
        const wchar_t* filename,
        bool startupPriority,
        GFMainPhase5State& state)
    {
        directory += childDirectory;
        OpenBank(directory, filename, startupPriority, state);
    }

    void SetCompiledDefinitionsDirectory(
        GFMainPhase5State& state,
        const wchar_t* pathname)
    {
        wcsncpy(
            state.compiledDefinitionsDirectory,
            pathname,
            sizeof(state.compiledDefinitionsDirectory) /
                sizeof(state.compiledDefinitionsDirectory[0]) - 1);
        state.compiledDefinitionsDirectory[
            sizeof(state.compiledDefinitionsDirectory) /
                sizeof(state.compiledDefinitionsDirectory[0]) - 1] = L'\0';
    }
}

long FABLE_FASTCALL FableRunGFMainPhase5(GFMainPhase5State& state)
{
    if (g_FableNavigatorEnabled_013B8616 != 0)
    {
        if (g_FableOpenAllRetailBanks_013B8605 != 0)
        {
            OpenBank(
                FableGetGraphicsDirectory_0041A100(),
                L"graphics.big",
                true,
                state);
            OpenNestedBank(
                FableGetGraphicsDirectory_0041A100(),
                L"pc\\",
                L"textures.big",
                true,
                state);
            OpenBank(
                FableGetLanguageDirectoryB_0041A330(),
                L"dialogue.big",
                false,
                state);
            OpenBank(
                FableGetMiscDirectoryB_0041A0C0(),
                L"effects.big",
                false,
                state);
            OpenBank(
                FableGetLanguageDirectoryB_0041A330(),
                L"text.big",
                false,
                state);
            OpenBank(
                FableGetMiscDirectoryA_0041A0A0(),
                L"temp.big",
                false,
                state);
            OpenBank(
                FableGetShadersDirectory_0041A1E0(),
                L"shaders.big",
                false,
                state);
        }

        OpenBank(
            FableGetLanguageDirectoryA_0041A120(),
            L"fonts.big",
            true,
            state);
        OpenBank(
            FableGetLanguageDirectoryB_0041A330(),
            L"text.big",
            false,
            state);

        FableGFMainPhase5SetHeaderDirectoryBoundary(
            L"data\\defs\\RetailHeaders\\",
            state);
        SetCompiledDefinitionsDirectory(
            state,
            L"Data\\CompiledDefs\\");
    }
    else
    {
        FableGFMainPhase5OpenIniFileBoundary(
            g_FableUseDvdBankList_013B8615 != 0
                ? L"banks_dvd.ini"
                : L"banks.ini",
            state);
        FableGFMainPhase5SetHeaderDirectoryBoundary(
            L"data\\defs\\DevHeaders\\",
            state);
        SetCompiledDefinitionsDirectory(
            state,
            L"Data\\CompiledDefs\\Development\\");
    }

    return 0;
}
