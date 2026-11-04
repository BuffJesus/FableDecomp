#pragma optimize("s", on)
// NUISystem::CFrontEndManager::GetInstance @ 0x00595582 (41 bytes).
// FableWin ?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ: lazily
// creates the 0xE0-byte frontend manager singleton (m_pInstance, retail
// 0x013B8B5C).
#include "fable_nui_manager.h"

NUISystem::CFrontEndManager* FABLE_FASTCALL NUISystem::CFrontEndManager::GetInstance()
{
    if (m_pInstance == 0)
        m_pInstance = new CFrontEndManager;
    return m_pInstance;
}
