#pragma optimize("s", on)
// CNewFrontendGameComponent::InitialiseGraphics @ 0x0042DDB3
// FableWin.pdb: ?InitialiseGraphics@CNewFrontendGameComponent@@AAE_NXZ.
// Fetch the default frontend font, then open GBANK_FRONT_END with the best
// texture formats the display manager offers.

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include "fable_graphic_data_bank_init.h"

// Retail globals: the frontend TU's anonymous-namespace
// DEFAULT_FRONT_END_FONT (FableWin dynamic initializer 0x03FDE859) and the
// engine-wide manager pointers GFInitialise 0x004022B0 caches from
// CSystemManager::PFontManager / PGraphicBankManager.
extern const CCharString g_FableDefaultFrontEndFont_013B875C;
extern CFontManager* g_FableFontManager_013B838C;
extern CGraphicBankManager* g_FableGraphicBankManager_013B837C;

bool CNewFrontendGameComponent::InitialiseGraphics()
{
    m_pMenuFont = g_FableFontManager_013B838C->GetFont(
        g_FableDefaultFrontEndFont_013B875C);

    CGraphicDataBankInit init;
    GFGetSystemManager()->GetDisplayManager()->GetBestCompressedOpaqueTextureFormat(
        32, init.NonAlphaFormat);
    GFGetSystemManager()->GetDisplayManager()->GetBestCompressedAlphaTextureFormat(
        32, init.AlphaFormat);
    GFGetSystemManager()->GetDisplayManager()->GetBestCompressedBooleanAlphaTextureFormat(
        16, init.BooleanAlphaFormat);
    GFGetSystemManager()->GetDisplayManager()->GetBestAlphaTextureFormat(
        32, init.UncompressedAlphaFormat);
    GFGetSystemManager()->GetDisplayManager()->GetBestOpaqueTextureFormat(
        32, init.UncompressedNonAlphaFormat, false);
    GFGetSystemManager()->GetDisplayManager()->GetBestSignedTextureFormat(
        16, init.SignedFormat);
    init.InterpolatedAlphaFormat.Initialise(D3DFMT_DXT5);
    init.GenerateMipmaps = true;
    init.MaxGraphicWidth = 2048;
    init.MaxGraphicHeight = 2048;
    init.AllowDither = true;

    m_pFrontEndGraphicBank = g_FableGraphicBankManager_013B837C->OpenGraphicBank(
        "GBANK_FRONT_END", init, true, true);
    return true;
}