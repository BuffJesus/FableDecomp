// Native frame coordinator at0042DF9E: behavior-gated NEAR_MATCH.
// CRT _controlfp preserves the retail rounding-to-nearest change without asm.
#pragma optimize("s",on)
#include <float.h>
#include "fable_game.h"
#include "fable_iengine.h"
#include "fable_nui_manager.h"
#include "fable_ime.h"
#include "fable_rgb_colour.h"
extern CRenderManager2D* g_FableRenderManager2D_013B8384;
extern CDisplayManager* g_FableDisplayManager_013B8390;
extern double g_FableVirtualWidth_013961E8;
extern double g_FableVirtualHeight_013961F0;

void CNewFrontendGameComponent::Draw(const CEngineInterpolationInfo& info)
{
    CInterpolationInfo unusedInterpolation;
    m_fLastRenderTime=(float)GFGetTime();
    C2DBoxF window;
    window.left=0.0f;window.top=0.0f;
    window.right=(float)g_FableVirtualWidth_013961E8;
    window.bottom=(float)g_FableVirtualHeight_013961F0;
    _controlfp(_RC_NEAR,_MCW_RC);
    g_FableRenderManager2D_013B8384->SetFogEnable(false);
    g_FableRenderManager2D_013B8384->SetVWindow(window);
    C2DBoxF visible=g_FableDisplayManager_013B8390->GetVVisibleTelevisionScreenArea();
    g_FableRenderManager2D_013B8384->ClearRenderTarget(CRGBColour(0,0,0),0);
    g_FableDisplayManager_013B8390->BeginRender();
    NUISystem::CFrontEndManager::GetInstance()->Draw(*m_pEngine,0);
    if(NUISystem::CManager::GetInstance()->HasErrorMessage())
        NUISystem::CManager::GetInstance()->RenderErrorMessage(*m_pEngine);
    m_pEngine->Render(info,0);
    g_FableRenderManager2D_013B8384->SetTextureMipMapMode(0,TEXTURE_MIPMAP_LINEAR);
    g_FableRenderManager2D_013B8384->DrawBufferedPolys();
    g_FableRenderManager2D_013B8384->Render2DDrawList(true);
    NUISystem::CIME::Get().Render();
    g_FableRenderManager2D_013B8384->DrawBufferedPolys();
    g_FableRenderManager2D_013B8384->Render2DDrawList(true);
    g_FableDisplayManager_013B8390->EndRender();
    g_FableDisplayManager_013B8390->SwapScreens();
}
