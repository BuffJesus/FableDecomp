#include "fable_boot.h"

struct CDisplayManager;

#pragma pack(push, 1)
struct FableEngineRoot
{
    fable_u8 unknown00[0x58];
    fable_u32 renderDevice58;
    fable_u8 unknown5C[0x04];
    CDisplayManager* displayManager60;
    fable_u8 unknown64[0x08];
    fable_u32 textureManager6C;
    fable_u8 unknown70[0x0C];
    fable_u32 primitiveManager7C;
    fable_u8 unknown80[0x04];
    fable_u32 resourceManager84;
};

struct FableDisplayPrimitive
{
    fable_u8 unknown00[0x3AB0];
    fable_u32 maximumWidth3AB0;
    fable_u32 maximumHeight3AB4;
};

struct CDisplayManager
{
    fable_u8 unknown00[0x08];
    FableDisplayPrimitive* displayPrimitive08;
    fable_u8 unknown0C[0x1BC];
    fable_i32 displayWidth1C8;

    bool IsTextureBitDepthSupported(int bitDepth);
};
#pragma pack(pop)

struct CXMVPlayer
{
    static void GlobalRegisterPlayer();
};

extern FableEngineRoot* FABLE_FASTCALL AddChildPrimitive();
extern void FABLE_CDECL GFGetBuildNumber2();

extern fable_u32 g_FablePrimitiveManager_013B8394;
extern CDisplayManager* g_FableDisplayManager_013B8390;
extern FableDisplayPrimitive* g_FableDisplayPrimitive_013B8384;
extern void* g_FableRenderState_013B8380;
extern fable_u8 g_FableRenderStateStorage_013BC470;
extern fable_u32 g_FableRenderDevice_013B8388;
extern fable_u32 g_FableResourceManager_013B838C;
extern fable_u32 g_FableTextureManager_013B837C;
extern fable_u32 g_FableTextureBitDepth_013B7D68;
extern fable_u32 g_FableRequestedTextureBitDepth_01375470;
extern fable_u32 g_FableMaximumDisplayDimension_0137546C;
extern fable_u32 g_FableDisplayWidth_013B7D64;
extern fable_u32 g_FableDisplayHeight_013B7D60;
extern fable_i32 g_FableHalfDisplayWidth_0137544C;
extern GFInitialiseState g_GFInitialiseState_013B83D0;

bool FABLE_FASTCALL GFInitialise()
{
    g_FablePrimitiveManager_013B8394 =
        AddChildPrimitive()->primitiveManager7C;

    g_FableDisplayManager_013B8390 =
        AddChildPrimitive()->displayManager60;
    g_FableDisplayPrimitive_013B8384 =
        g_FableDisplayManager_013B8390->displayPrimitive08;
    g_FableRenderState_013B8380 = &g_FableRenderStateStorage_013BC470;

    g_FableRenderDevice_013B8388 =
        AddChildPrimitive()->renderDevice58;
    g_FableResourceManager_013B838C =
        AddChildPrimitive()->resourceManager84;
    g_FableTextureManager_013B837C =
        AddChildPrimitive()->textureManager6C;

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

    FableDisplayPrimitive* const displayPrimitive =
        g_FableDisplayPrimitive_013B8384;
    fable_u32 displayDimension =
        displayPrimitive->maximumWidth3AB0;
    if (
        g_FableMaximumDisplayDimension_0137546C <
        displayDimension)
    {
        displayDimension =
            g_FableMaximumDisplayDimension_0137546C;
    }
    g_FableDisplayWidth_013B7D64 = displayDimension;

    displayDimension =
        displayPrimitive->maximumHeight3AB4;
    if (
        g_FableMaximumDisplayDimension_0137546C <
        displayDimension)
    {
        displayDimension =
            g_FableMaximumDisplayDimension_0137546C;
    }
    g_FableDisplayHeight_013B7D60 = displayDimension;

    CXMVPlayer::GlobalRegisterPlayer();
    GFGetBuildNumber2();

    if (GFInitialise_SetupProgressDisplay(
            &g_GFInitialiseState_013B83D0) == 1)
    {
        g_FableHalfDisplayWidth_0137544C =
            g_FableDisplayManager_013B8390->displayWidth1C8 / 2;
        return true;
    }

    return false;
}