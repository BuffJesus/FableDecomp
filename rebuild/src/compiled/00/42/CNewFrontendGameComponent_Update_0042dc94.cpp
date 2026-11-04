#pragma optimize("s", on)
// CNewFrontendGameComponent::Update @ 0x0042DC94
// FableWin.pdb: ?Update@CNewFrontendGameComponent@@AAEXXZ. Clamp the real
// time step to 0.1 s, raise / clear the controller-disconnect error message,
// tick the frontend manager, then stamp the update time.

#include "fable_game.h"
#include "fable_nui_manager.h"

void CNewFrontendGameComponent::Update()
{
    float timePassed = 1.0f / 15.0f;
    timePassed = (float)(GFGetTime() - m_fLastRealUpdateTime);
    if (timePassed > 0.1f)
        timePassed = 0.1f;

    if (IsControllerDisconnected() && !m_pFrontEndManager->CanStartAttractMode())
    {
        if (!NUISystem::CManager::GetInstance()->HasErrorMessage())
            ShowErrorMessage(EMT_CONTROLLER_DISCONNECT);
    }
    else if (NUISystem::CManager::GetInstance()->HasErrorMessage())
    {
        NUISystem::CManager::GetInstance()->ClearErrorMessage();
    }

    m_pFrontEndManager->Update(timePassed);
    m_fLastRealUpdateTime = GFGetTime();
}