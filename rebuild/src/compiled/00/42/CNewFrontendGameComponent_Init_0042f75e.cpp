#include "fable_frontend_startup.h"

// Retail body is 47 bytes, ending at 0x0042F78D; later alignment and leaves
// before the next catalogued function are not part of Init.
void CNewFrontendGameComponent::Init()
{
    g_FableNewFrontend_013B871C = this;
    InitialiseDefs();
    StaticLoadXMVCode();
    XMVCodeLoaded = true;
    // ASCII grave accent and retail EInputKey 0x29, not screen dimensions.
    FableGetConsole_00414C90()->Initialise(
        0x60, 0x29, const_cast<CFontBank*>(m_pMenuFont.Data));
}
