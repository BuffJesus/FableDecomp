// CMainGameComponent::BeginInputSaving @ 0x00416268 (23 bytes).
// FableWin ?BeginInputSaving@CMainGameComponent@@IAEXABVCWideString@@@Z.
// Starts recording the input stream to fileName, stamped with GVersion.
// (Retail 0x00416268 calls CInputManager::StreamInputsToFile; Ego_r's
// BeginInputSaving 0x004154CA does the same and precedes BeginInputLoading.)
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"

void CMainGameComponent::BeginInputSaving(const CWideString& fileName)
{
    g_FableInputManager_013B8388->StreamInputsToFile(g_Version_013B85E4, fileName);
}
