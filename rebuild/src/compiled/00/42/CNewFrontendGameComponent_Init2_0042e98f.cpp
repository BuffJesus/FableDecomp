#pragma optimize("s", on)
// CNewFrontendGameComponent::Init2 @ 0x0042E98F (256 bytes).
// FableWin ?Init2@CNewFrontendGameComponent@@AAEXXZ. Hooks the component up
// to the frontend manager singleton, brings up graphics / sounds / input,
// opens the frontend text bank, initialises the manager with the frontend
// banks and builds its screens, then asks for a 1024x768x16 display mode.
// (FableWin's debug-only "no frontend manager" error box is compiled out.)
#include "fable_game.h"
#include "fable_nui_manager.h"
#include "fable_particle_engine.h"
#include "fable_game_text_bank.h"
#include "fable_definition_manager.h"
#include "fable_display_viewport.h"
#include "fable_surface.h"

// ?GShowInitialMovies@@3_NA (retail 0x01375448); named as in
// CNewFrontendGameComponent::Run 0x0042EC7C.
extern bool g_FableFrontendFirstRun_01375448;

void CNewFrontendGameComponent::Init2()
{
    m_pFrontEndManager = NUISystem::CFrontEndManager::GetInstance();
    m_pFrontEndManager->SetGameComponent(this);
    m_pFrontEndManager->SetQuitFromGame(!g_FableFrontendFirstRun_01375448);

    InitialiseGraphics();
    InitialiseSounds();
    InitialiseInput();

    NParticleEngine::SetGraphicBank(*m_pFrontEndGraphicBank.Get());

    m_pTextBank.reset(new NGameText::CDataBank);
    m_pTextBank.Get()->OpenReadOnly(NLocalisation::GetTextBankName(), 0x100);

    NUISystem::CManager::Clear();

    m_pFrontEndManager->Init(m_pFrontEndGraphicBank, m_pMeshBank, m_pInputManager,
                             &CStartupDefinitionManager::Get());
    m_pFrontEndManager->BuildComponents(!unknown144_);

    g_FableDisplayManager_013B8390->SetDisplayMode(
        C2DExtentsI(1024, 768), 16, SURFACE_MULTISAMPLE_NONE, 60);
}
