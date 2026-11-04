// CMainGameComponent::BeginInputLoading @ 0x0041627F (23 bytes).
// FableWin ?BeginInputLoading@CMainGameComponent@@IAEXABVCWideString@@@Z.
// Starts replaying a recorded input stream from fileName, checked against
// GVersion. (Retail 0x0041627F calls CInputManager::StreamInputsFromFile.)
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"

void CMainGameComponent::BeginInputLoading(const CWideString& fileName)
{
    g_FableInputManager_013B8388->StreamInputsFromFile(g_Version_013B85E4, fileName);
}
