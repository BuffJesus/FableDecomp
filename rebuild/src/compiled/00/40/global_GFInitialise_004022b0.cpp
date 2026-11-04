#include "fable_boot.h"
#include "fable_cgame.h"
#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"
#include "fable_shader_render_manager.h"

struct CXMVPlayer
{
    static void GlobalRegisterPlayer();
};

extern void FABLE_CDECL GFGetBuildNumber2();

extern void* g_FableRenderState_013B8380;
extern unsigned long g_FableTextureBitDepth_013B7D68;
extern unsigned long g_FableRequestedTextureBitDepth_01375470;
extern unsigned long g_FableMaximumTextureSize_0137546C;
extern unsigned long g_FableMaxTextureWidth_013B7D64;
extern unsigned long g_FableMaxTextureHeight_013B7D60;
extern long g_FableHalfRefreshRate_0137544C;

bool FABLE_FASTCALL GFInitialise()
{
    g_FableSoundManager_013B8394 =
        GFGetSystemManager()->PSoundManager.Data;

    g_FableDisplayManager_013B8390 =
        GFGetSystemManager()->PDisplayManager.Data;
    g_FableRenderManager2D_013B8384 =
        g_FableDisplayManager_013B8390->PRenderManager.get();
    g_FableRenderState_013B8380 = &FableShaderRenderManager_013bc470;

    g_FableInputManager_013B8388 =
        GFGetSystemManager()->PInputManager.Data;
    g_FableFontManager_013B838C =
        GFGetSystemManager()->PFontManager.Data;
    g_FableGraphicBankManager_013B837C =
        GFGetSystemManager()->PGraphicBankManager.Data;

    g_FableTextureBitDepth_013B7D68 =
        g_FableRequestedTextureBitDepth_01375470;
    if (g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(
            g_FableRequestedTextureBitDepth_01375470))
    {
        g_FableTextureBitDepth_013B7D68 =
            g_FableRequestedTextureBitDepth_01375470;
    }
    else if (
        g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(16))
    {
        g_FableTextureBitDepth_013B7D68 = 16;
    }
    else if (
        g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(24))
    {
        g_FableTextureBitDepth_013B7D68 = 24;
    }
    else if (
        g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(32))
    {
        g_FableTextureBitDepth_013B7D68 = 32;
    }

    // Texture limits are the device's, capped by the configured maximum.
    CRenderManager2D* const renderManager =
        g_FableRenderManager2D_013B8384;
    unsigned long textureSize =
        renderManager->D3DDeviceCaps.MaxTextureWidth;
    if (g_FableMaximumTextureSize_0137546C < textureSize)
    {
        textureSize = g_FableMaximumTextureSize_0137546C;
    }
    g_FableMaxTextureWidth_013B7D64 = textureSize;

    textureSize = renderManager->D3DDeviceCaps.MaxTextureHeight;
    if (g_FableMaximumTextureSize_0137546C < textureSize)
    {
        textureSize = g_FableMaximumTextureSize_0137546C;
    }
    g_FableMaxTextureHeight_013B7D60 = textureSize;

    CXMVPlayer::GlobalRegisterPlayer();
    GFGetBuildNumber2();

    if (g_Game_013B83D0.Initialise() == true)
    {
        g_FableHalfRefreshRate_0137544C =
            g_FableDisplayManager_013B8390->RefreshRate / 2;
        return true;
    }

    return false;
}
