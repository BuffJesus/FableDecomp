#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_gfmain.h"

namespace
{
    void ResetState(GFMainPhase6State& state)
    {
        memset(&state, 0, sizeof(state));
        wcscpy(state.myDocumentsDirectory, L"C:\\Users\\Hero\\Documents\\");
    }

    bool HasNormalPaths(const GFMainPhase6State& state)
    {
        return
            wcscmp(
                state.definitionTablePath,
                L"Data\\CompiledDefs\\names.bin") == 0 &&
            wcscmp(
                state.checkpointDirectory,
                L"C:\\Users\\Hero\\Documents\\My Games\\Fable\\") == 0 &&
            wcscmp(
                state.saveDirectory,
                L"C:\\Users\\Hero\\Documents\\My Games\\Fable\\Saves\\") == 0;
    }
}

int main()
{
    GFMainPhase5State phase5;
    memset(&phase5, 0, sizeof(phase5));
    wcscpy(
        phase5.compiledDefinitionsDirectory,
        L"Data\\CompiledDefs\\");

    GFMainPhase6State state;
    ResetState(state);
    state.definitionTableEnabled = true;
    state.checkpointOpenSucceeds = true;
    state.saveOpenSucceeds = true;

    if (
        FableRunGFMainPhase6(phase5, state) != 0 ||
        !HasNormalPaths(state) ||
        state.setDefinitionTablePathCalls != 1 ||
        state.loadDefinitionTableCalls != 1 ||
        !state.definitionTableLoadAllowed ||
        state.exactDefinitionTableSetPathCalls != 0 ||
        state.exactDefinitionTableLoadCalls != 0 ||
        state.exactDefinitionTablePathExistsCalls != 0 ||
        state.exactPermissionProbeCalls != 0 ||
        state.exactPermissionCreateCalls != 0 ||
        state.openDirectoryCalls[GFMainPhase6Path_Checkpoint] != 1 ||
        state.openDirectoryCalls[GFMainPhase6Path_Save] != 1 ||
        state.closeDirectoryCalls[GFMainPhase6Path_Checkpoint] != 1 ||
        state.closeDirectoryCalls[GFMainPhase6Path_Save] != 1 ||
        state.permissionCheckCalls[GFMainPhase6Path_Checkpoint] != 1 ||
        state.permissionCheckCalls[GFMainPhase6Path_Save] != 1 ||
        state.displayPermissionErrorCalls != 0)
    {
        puts("FABLETLC_GFMAIN_PHASE6_BEHAVIOR FAIL code=1");
        return 1;
    }

    ResetState(state);
    state.definitionTableEnabled = true;
    state.definitionTableLoadDisabled = true;
    state.checkpointOpenSucceeds = false;
    state.myDocumentsPermissionResult = 42;
    state.permissionTextLookupResult = 101;
    state.captionTextLookupResult = 202;
    state.permissionGroupAvailable = true;
    state.captionGroupAvailable = false;

    if (
        FableRunGFMainPhase6(phase5, state) != 1 ||
        state.loadDefinitionTableCalls != 1 ||
        state.definitionTableLoadAllowed ||
        state.openDirectoryCalls[GFMainPhase6Path_Save] != 0 ||
        state.permissionCheckCalls[GFMainPhase6Path_MyDocuments] != 1 ||
        state.compressedStreamConstructCalls != 1 ||
        state.countedPointerResetCalls != 1 ||
        state.openFontBankCalls != 1 ||
        state.textLookupCalls[GFMainPhase6Text_PermissionError] != 1 ||
        state.textLookupCalls[GFMainPhase6Text_ErrorCaption] != 1 ||
        state.groupEntryCalls[GFMainPhase6Text_PermissionError] != 1 ||
        state.groupEntryCalls[GFMainPhase6Text_ErrorCaption] != 1 ||
        state.bankHandleCalls[GFMainPhase6Text_PermissionError] != 1 ||
        state.bankHandleCalls[GFMainPhase6Text_ErrorCaption] != 0 ||
        state.displayPermissionErrorCalls != 1 ||
        state.displayedPermissionError != 42)
    {
        puts("FABLETLC_GFMAIN_PHASE6_BEHAVIOR FAIL code=2");
        return 2;
    }

    ResetState(state);
    state.checkpointOpenSucceeds = true;
    state.checkpointPermissionResult = 99;
    state.saveOpenSucceeds = false;
    if (
        FableRunGFMainPhase6(phase5, state) != 1 ||
        state.displayedPermissionError != 99 ||
        state.permissionCheckCalls[GFMainPhase6Path_Save] != 0)
    {
        puts("FABLETLC_GFMAIN_PHASE6_BEHAVIOR FAIL code=3");
        return 3;
    }

    ResetState(state);
    state.checkpointOpenSucceeds = true;
    state.checkpointPermissionResult = 99;
    state.saveOpenSucceeds = true;
    state.savePermissionResult = 0;
    if (
        FableRunGFMainPhase6(phase5, state) != 0 ||
        state.displayPermissionErrorCalls != 0 ||
        state.permissionCheckCalls[GFMainPhase6Path_Save] != 1)
    {
        puts("FABLETLC_GFMAIN_PHASE6_BEHAVIOR FAIL code=4");
        return 4;
    }

    ResetState(state);
    state.checkpointOpenSucceeds = true;
    state.saveOpenSucceeds = true;
    state.savePermissionResult = 7;
    if (
        FableRunGFMainPhase6(phase5, state) != 1 ||
        state.loadDefinitionTableCalls != 0 ||
        state.displayedPermissionError != 7)
    {
        puts("FABLETLC_GFMAIN_PHASE6_BEHAVIOR FAIL code=5");
        return 5;
    }

    puts("FABLETLC_GFMAIN_PHASE6_BEHAVIOR PASS");
    return 0;
}
