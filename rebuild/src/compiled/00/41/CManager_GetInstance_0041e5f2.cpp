#pragma optimize("s", on)
// NUISystem::CManager::GetInstance @ 0x0041E5F2 (41 bytes).
// FableWin ?GetInstance@CManager@NUISystem@@SIPAV12@XZ: lazily creates the
// 0xD0-byte UI manager singleton (GPUniqueInstance, retail 0x013B8710).
#include "fable_nui_manager.h"

NUISystem::CManager* FABLE_FASTCALL NUISystem::CManager::GetInstance()
{
    if (GPUniqueInstance == 0)
        GPUniqueInstance = new CManager;
    return GPUniqueInstance;
}
