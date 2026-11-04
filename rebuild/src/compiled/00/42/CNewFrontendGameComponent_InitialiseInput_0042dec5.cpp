// CNewFrontendGameComponent::InitialiseInput @ 0x0042DEC5
// VC7.1, x86, /O2 /Oy.

#include "fable_game.h"
#include "fable_gfuninitialise.h"

void CNewFrontendGameComponent::InitialiseInput()
{
    m_pInputManager = GFGetSystemManager()->PInputManager.Data;
}
