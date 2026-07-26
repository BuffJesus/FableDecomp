#include <stdio.h>
#include <string.h>

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

extern bool FABLE_FASTCALL GFInitialise();

fable_u32 g_FablePrimitiveManager_013B8394;
CDisplayManager* g_FableDisplayManager_013B8390;
FableDisplayPrimitive* g_FableDisplayPrimitive_013B8384;
void* g_FableRenderState_013B8380;
fable_u8 g_FableRenderStateStorage_013BC470;
fable_u32 g_FableRenderDevice_013B8388;
fable_u32 g_FableResourceManager_013B838C;
fable_u32 g_FableTextureManager_013B837C;
fable_u32 g_FableTextureBitDepth_013B7D68;
fable_u32 g_FableRequestedTextureBitDepth_01375470;
fable_u32 g_FableMaximumDisplayDimension_0137546C;
fable_u32 g_FableDisplayWidth_013B7D64;
fable_u32 g_FableDisplayHeight_013B7D60;
fable_i32 g_FableHalfDisplayWidth_0137544C;
GFInitialiseState g_GFInitialiseState_013B83D0;

namespace
{
    FableEngineRoot g_engineRoot;
    FableDisplayPrimitive g_displayPrimitive;
    CDisplayManager g_displayManager;
    int g_rootCalls;
    int g_bitDepthCalls;
    int g_bitDepthArguments[4];
    int g_supportedBitDepth;
    int g_xmvCalls;
    int g_buildNumberCalls;
    int g_progressSetupCalls;
    bool g_progressSetupResult;
    GFInitialiseState* g_progressState;
}

FableEngineRoot* FABLE_FASTCALL AddChildPrimitive()
{
    ++g_rootCalls;
    return &g_engineRoot;
}

bool CDisplayManager::IsTextureBitDepthSupported(int bitDepth)
{
    g_bitDepthArguments[g_bitDepthCalls++] = bitDepth;
    return bitDepth == g_supportedBitDepth;
}

void CXMVPlayer::GlobalRegisterPlayer()
{
    ++g_xmvCalls;
}

void FABLE_CDECL GFGetBuildNumber2()
{
    ++g_buildNumberCalls;
}

fable_u8 FABLE_FASTCALL GFInitialise_SetupProgressDisplay(
    GFInitialiseState* state)
{
    ++g_progressSetupCalls;
    g_progressState = state;
    return g_progressSetupResult ? 1 : 0;
}

static void ResetFixture()
{
    memset(&g_engineRoot, 0, sizeof(g_engineRoot));
    memset(&g_displayPrimitive, 0, sizeof(g_displayPrimitive));
    memset(&g_displayManager, 0, sizeof(g_displayManager));

    g_engineRoot.renderDevice58 = 0x58585858;
    g_engineRoot.displayManager60 = &g_displayManager;
    g_engineRoot.textureManager6C = 0x6C6C6C6C;
    g_engineRoot.primitiveManager7C = 0x7C7C7C7C;
    g_engineRoot.resourceManager84 = 0x84848484;
    g_displayManager.displayPrimitive08 = &g_displayPrimitive;
    g_displayManager.displayWidth1C8 = 1365;
    g_displayPrimitive.maximumWidth3AB0 = 1920;
    g_displayPrimitive.maximumHeight3AB4 = 720;

    g_rootCalls = 0;
    g_bitDepthCalls = 0;
    g_supportedBitDepth = 24;
    g_xmvCalls = 0;
    g_buildNumberCalls = 0;
    g_progressSetupCalls = 0;
    g_progressSetupResult = true;
    g_progressState = 0;

    g_FableRequestedTextureBitDepth_01375470 = 8;
    g_FableMaximumDisplayDimension_0137546C = 1280;
}

static int VerifySuccessfulProgressSetup()
{
    ResetFixture();

    if (!GFInitialise())
        return 1;
    if (g_rootCalls != 5)
        return 2;
    if (
        g_FablePrimitiveManager_013B8394 != 0x7C7C7C7C ||
        g_FableRenderDevice_013B8388 != 0x58585858 ||
        g_FableResourceManager_013B838C != 0x84848484 ||
        g_FableTextureManager_013B837C != 0x6C6C6C6C)
        return 3;
    if (
        g_FableDisplayManager_013B8390 != &g_displayManager ||
        g_FableDisplayPrimitive_013B8384 != &g_displayPrimitive ||
        g_FableRenderState_013B8380 != &g_FableRenderStateStorage_013BC470)
        return 4;
    if (
        g_bitDepthCalls != 3 ||
        g_bitDepthArguments[0] != 8 ||
        g_bitDepthArguments[1] != 16 ||
        g_bitDepthArguments[2] != 24 ||
        g_FableTextureBitDepth_013B7D68 != 24)
        return 5;
    if (
        g_FableDisplayWidth_013B7D64 != 1280 ||
        g_FableDisplayHeight_013B7D60 != 720)
        return 6;
    if (
        g_xmvCalls != 1 ||
        g_buildNumberCalls != 1 ||
        g_progressSetupCalls != 1 ||
        g_progressState != &g_GFInitialiseState_013B83D0)
        return 7;
    if (g_FableHalfDisplayWidth_0137544C != 682)
        return 8;
    return 0;
}

static int VerifyRejectedProgressSetup()
{
    ResetFixture();
    g_progressSetupResult = false;

    if (GFInitialise())
        return 9;
    if (g_progressSetupCalls != 1)
        return 10;
    return 0;
}

int main()
{
    int result = VerifySuccessfulProgressSetup();
    if (result == 0)
        result = VerifyRejectedProgressSetup();

    if (result != 0)
    {
        printf("FABLETLC_GFINITIALISE_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_GFINITIALISE_BEHAVIOR PASS\n");
    return 0;
}
