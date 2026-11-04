#pragma optimize("s", on)
// ShowErrorMessage @ 0x0041E61B (14 bytes).
// FableWin ?ShowErrorMessage@@YIXW4ERRORMESSAGE_TYPE@@@Z: the global
// convenience wrapper that forwards to the UI manager singleton.
#include "fable_nui_manager.h"

void FABLE_FASTCALL ShowErrorMessage(ERRORMESSAGE_TYPE type)
{
    NUISystem::CManager::GetInstance()->ShowErrorMessage(type);
}
