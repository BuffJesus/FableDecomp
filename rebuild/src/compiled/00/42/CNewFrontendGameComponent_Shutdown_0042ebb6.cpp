// CNewFrontendGameComponent::Shutdown @ 0x0042EBB6 (198 bytes).
// Drops the frontend def, unpublishes the component, releases its banks and
// engine, lets fonts stream asynchronously again, quiets the sound manager
// (unless the main game is about to be created), unloads the XMV codec,
// empties the save lists and presents one black frame.
#pragma optimize("s", on)
#include "fable_definition_manager.h"
#include "fable_game.h"
#include "fable_rgb_colour.h"
#include "fable_system_manager.h"

extern CNewFrontendGameComponent* g_FableFrontendGameComponent_013B871C;
extern CFontManager* g_FableFontManager_013B838C;
extern CASoundManager* g_FableSoundManager_013B8394;

// Out of line in retail: vector<CWideString>::erase 0x00412450 and
// vector<CDateAndTime>::erase 0x00411B00.
template <> CWideString* _STL::vector<CWideString>::erase(CWideString* first, CWideString* last);
template <> CDateAndTime* _STL::vector<CDateAndTime>::erase(CDateAndTime* first, CDateAndTime* last);

void CNewFrontendGameComponent::Shutdown()
{
    m_pFrontEndDef.Reset(0);
    CStartupDefinitionManager::Set(0);
    g_FableFrontendGameComponent_013B871C = 0;
    CleanUpInterfaces();
    g_FableFontManager_013B838C->SetLoadFontGlyphsAsynchronously(true);

    if (!m_bCreateGame && g_FableSoundManager_013B8394)
    {
        g_FableSoundManager_013B8394->UnknownSlot16();
        g_FableSoundManager_013B8394->UnknownSlot18(0);
        g_FableSoundManager_013B8394->UnknownUpdateEntries_00991750();
        g_FableSoundManager_013B8394->UnknownClearEntries_009918F0();
    }

    ReleaseXMVCode();
    FileNames.clear();
    FileTimes.clear();

    if (g_FableDisplayManager_013B8390)
    {
        g_FableDisplayManager_013B8390->ClearRenderTarget(CRGBColour(0, 0, 0, 0xFF), 7);
        g_FableDisplayManager_013B8390->SwapScreens();
    }
}