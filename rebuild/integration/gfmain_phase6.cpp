#include <wchar.h>

#include "fable_gfmain.h"

namespace
{
    void CopyPath(
        wchar_t* destination,
        size_t capacity,
        const wchar_t* source)
    {
        wcsncpy(destination, source != 0 ? source : L"", capacity - 1);
        destination[capacity - 1] = L'\0';
    }

    void AppendPath(
        wchar_t* destination,
        size_t capacity,
        const wchar_t* suffix)
    {
        const size_t length = wcslen(destination);
        if (length + 1 < capacity)
        {
            wcsncat(
                destination,
                suffix,
                capacity - length - 1);
        }
        destination[capacity - 1] = L'\0';
    }

    void BuildDirectoryPaths(GFMainPhase6State& state)
    {
        const wchar_t* myDocuments =
            state.myDocumentsDirectory[0] != L'\0'
                ? state.myDocumentsDirectory
                : L"My Documents\\";

        CopyPath(
            state.checkpointDirectory,
            sizeof(state.checkpointDirectory) /
                sizeof(state.checkpointDirectory[0]),
            myDocuments);
        AppendPath(
            state.checkpointDirectory,
            sizeof(state.checkpointDirectory) /
                sizeof(state.checkpointDirectory[0]),
            L"My Games\\Fable\\");

        CopyPath(
            state.saveDirectory,
            sizeof(state.saveDirectory) /
                sizeof(state.saveDirectory[0]),
            state.checkpointDirectory);
        AppendPath(
            state.saveDirectory,
            sizeof(state.saveDirectory) /
                sizeof(state.saveDirectory[0]),
            L"Saves\\");
    }

    void ResolveTextHandle(
        GFMainPhase6TextRole role,
        GFMainPhase6State& state)
    {
        const fable_u32 identifier =
            FableGFMainPhase6LookupTextBoundary(role, state);
        if (
            identifier != 0 &&
            FableGFMainPhase6GetGroupEntryBoundary(
                role,
                identifier,
                state))
        {
            FableGFMainPhase6GetBankHandleBoundary(role, state);
        }
    }
}

long FABLE_FASTCALL FableRunGFMainPhase6(
    const GFMainPhase5State& phase5,
    GFMainPhase6State& state)
{
    wchar_t definitionTablePath[96];
    CopyPath(
        definitionTablePath,
        sizeof(definitionTablePath) /
            sizeof(definitionTablePath[0]),
        phase5.compiledDefinitionsDirectory);
    AppendPath(
        definitionTablePath,
        sizeof(definitionTablePath) /
            sizeof(definitionTablePath[0]),
        L"names.bin");
    FableGFMainPhase6SetDefinitionTablePathBoundary(
        definitionTablePath,
        state);

    if (state.definitionTableEnabled)
    {
        FableGFMainPhase6LoadDefinitionTableBoundary(
            !state.definitionTableLoadDisabled,
            state);
    }

    BuildDirectoryPaths(state);

    fable_u32 permissionResult;
    if (!FableGFMainPhase6OpenDirectoryBoundary(
            GFMainPhase6Path_Checkpoint,
            state.checkpointDirectory,
            state))
    {
        const wchar_t* myDocuments =
            state.myDocumentsDirectory[0] != L'\0'
                ? state.myDocumentsDirectory
                : L"My Documents\\";
        permissionResult =
            FableGFMainPhase6CheckPermissionsBoundary(
                GFMainPhase6Path_MyDocuments,
                myDocuments,
                state);
    }
    else
    {
        FableGFMainPhase6CloseDirectoryBoundary(
            GFMainPhase6Path_Checkpoint,
            state);
        permissionResult =
            FableGFMainPhase6CheckPermissionsBoundary(
                GFMainPhase6Path_Checkpoint,
                state.checkpointDirectory,
                state);

        if (FableGFMainPhase6OpenDirectoryBoundary(
                GFMainPhase6Path_Save,
                state.saveDirectory,
                state))
        {
            FableGFMainPhase6CloseDirectoryBoundary(
                GFMainPhase6Path_Save,
                state);
            permissionResult =
                FableGFMainPhase6CheckPermissionsBoundary(
                    GFMainPhase6Path_Save,
                    state.saveDirectory,
                    state);
        }
    }

    if (permissionResult == 0)
        return 0;

    FableGFMainPhase6ConstructCompressedStreamBoundary(state);
    FableGFMainPhase6ResetCountedPointerBoundary(state);
    FableGFMainPhase6OpenFontBankBoundary(state);
    ResolveTextHandle(GFMainPhase6Text_PermissionError, state);
    ResolveTextHandle(GFMainPhase6Text_ErrorCaption, state);
    FableGFMainPhase6DisplayPermissionErrorBoundary(
        permissionResult,
        state);
    return 1;
}
