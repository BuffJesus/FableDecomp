// CMainGameComponent::InitGraphics @ 0x00416C8A
// FableWin.pdb: ?InitGraphics@CMainGameComponent@@IAE_NXZ.
// Pick the texture formats the display manager offers (compressed ones when
// the device supports them and GUseCompressedTextures is on), open
// GBANK_MAIN with them and make it the graphic-appearance bank.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_graphic_appearance.h"
#include "fable_graphic_data_bank_init.h"
#include "fable_progress_display.h"
#include "fable_system_manager.h"

// FableWin GUseCompressedTextures / GDitheringOn (console variables) and the
// texture limits GFInitialise 0x004022B0 settles (GTextureBitDepth,
// GMaxTextureWidth, GMaxTextureHeight).
extern bool g_FableUseCompressedTextures_01375453;
extern bool g_FableDitheringOn_01375452;
extern unsigned long g_FableTextureBitDepth_013B7D68;
extern unsigned long g_FableMaxTextureWidth_013B7D64;
extern unsigned long g_FableMaxTextureHeight_013B7D60;

bool CMainGameComponent::InitGraphics()
{
    NProgressDisplay::DisplayProgress("Init Graphics", -1.0f, false, false);

    CGraphicDataBankInit init;
    if (g_FableDisplayManager_013B8390->IsSupportingCompressedTextures()
        && g_FableUseCompressedTextures_01375453)
    {
        g_FableDisplayManager_013B8390->GetBestCompressedOpaqueTextureFormat(
            g_FableTextureBitDepth_013B7D68, init.NonAlphaFormat);
        g_FableDisplayManager_013B8390->GetBestCompressedAlphaTextureFormat(
            g_FableTextureBitDepth_013B7D68, init.AlphaFormat);
        g_FableDisplayManager_013B8390->GetBestCompressedBooleanAlphaTextureFormat(
            g_FableTextureBitDepth_013B7D68, init.BooleanAlphaFormat);
        init.InterpolatedAlphaFormat.Initialise(D3DFMT_DXT5);
    }
    else
    {
        g_FableDisplayManager_013B8390->GetBestOpaqueTextureFormat(
            g_FableTextureBitDepth_013B7D68, init.NonAlphaFormat, false);
        g_FableDisplayManager_013B8390->GetBestAlphaTextureFormat(
            g_FableTextureBitDepth_013B7D68, init.AlphaFormat);
        g_FableDisplayManager_013B8390->GetBestBooleanAlphaTextureFormat(
            g_FableTextureBitDepth_013B7D68, init.BooleanAlphaFormat);
    }
    g_FableDisplayManager_013B8390->GetBestAlphaTextureFormat(
        g_FableTextureBitDepth_013B7D68, init.UncompressedAlphaFormat);
    g_FableDisplayManager_013B8390->GetBestOpaqueTextureFormat(
        g_FableTextureBitDepth_013B7D68, init.UncompressedNonAlphaFormat, false);
    g_FableDisplayManager_013B8390->GetBestSignedTextureFormat(
        g_FableTextureBitDepth_013B7D68, init.SignedFormat);
    init.GenerateMipmaps = true;
    init.MaxGraphicWidth = g_FableMaxTextureWidth_013B7D64;
    init.MaxGraphicHeight = g_FableMaxTextureHeight_013B7D60;
    init.AllowDither = g_FableDitheringOn_01375452;

    NProgressDisplay::DisplayProgress("Opening Main Graphic Bank", -1.0f, false, false);
    PMainGraphicBank = g_FableGraphicBankManager_013B837C->OpenGraphicBank(
        "GBANK_MAIN", init, true, true);

    NProgressDisplay::DisplayProgress("Setting Main Graphic Bank", -1.0f, false, false);
    NGraphicAppearance::CTCGraphicAppearance::SetGraphicBank(*PMainGraphicBank.Get());
    return true;
}