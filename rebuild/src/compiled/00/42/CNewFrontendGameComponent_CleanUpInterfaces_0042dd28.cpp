// CNewFrontendGameComponent::CleanUpInterfaces @ 0x0042DD28 (139 bytes).
// FableWin ?CleanUpInterfaces@CNewFrontendGameComponent@@AAEXXZ. Called by
// Shutdown (0x0042EBE6) and Run (0x0042F247). Deletes the frontend manager,
// drops the engine, hands the graphic and mesh banks back to their managers
// and releases the menu font.
#pragma optimize("s", on)
#include "fable_game.h"
#include "fable_nui_manager.h"
#include "fable_system_manager.h"

extern CGraphicBankManager* g_FableGraphicBankManager_013B837C;

void CNewFrontendGameComponent::CleanUpInterfaces()
{
    if (m_pFrontEndManager)
    {
        delete m_pFrontEndManager;
        m_pFrontEndManager = 0;
    }

    if (!m_pEngine.IsNull())
    {
        m_pEngine.ReleaseData();
        m_pEngine.Data = 0;
    }

    if (!m_pFrontEndGraphicBank.IsNull())
    {
        g_FableGraphicBankManager_013B837C->CloseGraphicBank(m_pFrontEndGraphicBank);
        m_pFrontEndGraphicBank.ReleaseData();
        m_pFrontEndGraphicBank.Data = 0;
    }

    if (!m_pMeshBank.IsNull())
    {
        CSystemManager* systemManager = GFGetSystemManager();
        CMeshBankManager* meshBankManager = systemManager->PMeshBankManager.Data;
        meshBankManager->CloseMeshBank(m_pMeshBank);
        m_pMeshBank.ReleaseData();
        m_pMeshBank.Data = 0;
    }

    m_pMenuFont.ReleaseData();
    m_pMenuFont.Data = 0;
}