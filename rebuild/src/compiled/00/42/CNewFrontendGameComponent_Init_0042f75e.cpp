// CNewFrontendGameComponent::Init @ 0x0042F75E (47 bytes).
// Publishes the active frontend before loading definitions and initializing the console.
#pragma optimize("s", on)
#include "fable_game.h"
#include "fable_console.h"
extern CNewFrontendGameComponent* g_FableFrontendGameComponent_013B871C;
void FABLE_FASTCALL StaticLoadXMVCode();
void CNewFrontendGameComponent::Init()
{
    g_FableFrontendGameComponent_013B871C = this;
    InitialiseDefs();
    StaticLoadXMVCode();
    XMVCodeLoaded = true;
    CTBaseSingleton<CConsole>::Get().Initialise('`', EInputKey(0x29), m_pMenuFont.Data);
}