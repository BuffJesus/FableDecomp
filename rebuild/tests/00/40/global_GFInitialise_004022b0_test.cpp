#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_boot.h"
#include "fable_cgame.h"
#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"
#include "test_support/init_base_class_stubs.h"

struct CXMVPlayer
{
    static void GlobalRegisterPlayer();
};

extern bool FABLE_FASTCALL GFInitialise();

CASoundManager* g_FableSoundManager_013B8394;
CDisplayManager* g_FableDisplayManager_013B8390;
CRenderManager2D* g_FableRenderManager2D_013B8384;
void* g_FableRenderState_013B8380;
// This focused parent test supplies storage at its shader-manager boundary.
// Actual global construction/destruction is covered by the native global gate.
extern "C" { unsigned char InitShaderStorage[1000]={0}; }
#pragma comment(linker,"/alternatename:?FableShaderRenderManager_013bc470@@3VCShaderRenderManager@@A=_InitShaderStorage")
#define g_FableRenderStateStorage_013BC470 InitShaderStorage[0]
CInputManager* g_FableInputManager_013B8388;
CFontManager* g_FableFontManager_013B838C;
CGraphicBankManager* g_FableGraphicBankManager_013B837C;
unsigned long g_FableTextureBitDepth_013B7D68;
unsigned long g_FableRequestedTextureBitDepth_01375470;
unsigned long g_FableMaximumTextureSize_0137546C;
unsigned long g_FableMaxTextureWidth_013B7D64;
unsigned long g_FableMaxTextureHeight_013B7D60;
long g_FableHalfRefreshRate_0137544C;
CGame g_Game_013B83D0;

namespace
{
    // The engine objects, viewed through the shared classes without running
    // their constructors.
    union
    {
        double align;
        unsigned char bytes[sizeof(CSystemManager)];
    } g_systemStorage;
    union
    {
        double align;
        unsigned char bytes[sizeof(CDisplayManager)];
    } g_displayStorage;
    union
    {
        double align;
        unsigned char bytes[sizeof(CRenderManager2D)];
    } g_renderStorage;

    CSystemManager* const g_system =
        reinterpret_cast<CSystemManager*>(g_systemStorage.bytes);
    CDisplayManager* const g_display =
        reinterpret_cast<CDisplayManager*>(g_displayStorage.bytes);
    CRenderManager2D* const g_render =
        reinterpret_cast<CRenderManager2D*>(g_renderStorage.bytes);

    // Distinct addresses standing in for the other managers.
    unsigned char g_sound, g_input, g_font, g_graphicBanks;

    int g_systemCalls;
    int g_bitDepthCalls;
    unsigned long g_bitDepthArguments[4];
    const CDisplayManager* g_bitDepthReceiver;
    unsigned long g_supportedBitDepth;
    int g_xmvCalls;
    int g_buildNumberCalls;
    int g_gameCalls;
    bool g_gameResult;
    const CGame* g_gameReceiver;
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    ++g_systemCalls;
    return g_system;
}

bool CDisplayManager::IsTextureBitDepthSupported(unsigned long bitDepth)
{
    g_bitDepthReceiver = this;
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

bool CGame::Initialise()
{
    ++g_gameCalls;
    g_gameReceiver = this;
    return g_gameResult;
}

static void ResetFixture(unsigned long maxWidth, unsigned long maxHeight, long refreshRate)
{
    memset(g_systemStorage.bytes, 0, sizeof(g_systemStorage.bytes));
    memset(g_displayStorage.bytes, 0, sizeof(g_displayStorage.bytes));
    memset(g_renderStorage.bytes, 0, sizeof(g_renderStorage.bytes));

    g_system->PSoundManager.Data = reinterpret_cast<CASoundManager*>(&g_sound);
    g_system->PDisplayManager.Data = g_display;
    g_system->PInputManager.Data = reinterpret_cast<CInputManager*>(&g_input);
    g_system->PFontManager.Data = reinterpret_cast<CFontManager*>(&g_font);
    g_system->PGraphicBankManager.Data =
        reinterpret_cast<CGraphicBankManager*>(&g_graphicBanks);
    new (&g_display->PRenderManager) boost::scoped_ptr<CRenderManager>(reinterpret_cast<CRenderManager*>(g_render));
    g_display->RefreshRate = refreshRate;
    g_render->D3DDeviceCaps.MaxTextureWidth = maxWidth;
    g_render->D3DDeviceCaps.MaxTextureHeight = maxHeight;

    g_systemCalls = 0;
    g_bitDepthCalls = 0;
    g_bitDepthReceiver = 0;
    g_supportedBitDepth = 24;
    g_xmvCalls = 0;
    g_buildNumberCalls = 0;
    g_gameCalls = 0;
    g_gameResult = true;
    g_gameReceiver = 0;

    g_FableRequestedTextureBitDepth_01375470 = 8;
    g_FableMaximumTextureSize_0137546C = 1280;
    g_FableHalfRefreshRate_0137544C = -1;
}

static int VerifySuccessfulInitialise()
{
    ResetFixture(1920, 720, 75);

    if (!GFInitialise())
        return 1;
    if (g_systemCalls != 5)
        return 2;
    if (
        g_FableSoundManager_013B8394 != reinterpret_cast<CASoundManager*>(&g_sound) ||
        g_FableInputManager_013B8388 != reinterpret_cast<CInputManager*>(&g_input) ||
        g_FableFontManager_013B838C != reinterpret_cast<CFontManager*>(&g_font) ||
        g_FableGraphicBankManager_013B837C !=
            reinterpret_cast<CGraphicBankManager*>(&g_graphicBanks))
        return 3;
    if (
        g_FableDisplayManager_013B8390 != g_display ||
        g_FableRenderManager2D_013B8384 != g_render ||
        g_FableRenderState_013B8380 != &g_FableRenderStateStorage_013BC470)
        return 4;
    if (
        g_bitDepthCalls != 3 ||
        g_bitDepthReceiver != g_display ||
        g_bitDepthArguments[0] != 8 ||
        g_bitDepthArguments[1] != 16 ||
        g_bitDepthArguments[2] != 24 ||
        g_FableTextureBitDepth_013B7D68 != 24)
        return 5;
    // Width is capped by the configured maximum; height is within it.
    if (
        g_FableMaxTextureWidth_013B7D64 != 1280 ||
        g_FableMaxTextureHeight_013B7D60 != 720)
        return 6;
    if (
        g_xmvCalls != 1 ||
        g_buildNumberCalls != 1 ||
        g_gameCalls != 1 ||
        g_gameReceiver != &g_Game_013B83D0)
        return 7;
    if (g_FableHalfRefreshRate_0137544C != 37)
        return 8;
    return 0;
}

static int VerifyRequestedDepthAndSmallDevice()
{
    ResetFixture(512, 256, 60);
    g_FableRequestedTextureBitDepth_01375470 = 32;
    g_supportedBitDepth = 32;

    if (!GFInitialise())
        return 9;
    if (g_bitDepthCalls != 1 || g_FableTextureBitDepth_013B7D68 != 32)
        return 10;
    if (
        g_FableMaxTextureWidth_013B7D64 != 512 ||
        g_FableMaxTextureHeight_013B7D60 != 256)
        return 11;
    if (g_FableHalfRefreshRate_0137544C != 30)
        return 12;
    return 0;
}

static int VerifyRejectedGameInitialise()
{
    ResetFixture(1920, 720, 75);
    g_gameResult = false;

    if (GFInitialise())
        return 13;
    if (g_gameCalls != 1)
        return 14;
    // The refresh-rate cache is only written on success.
    if (g_FableHalfRefreshRate_0137544C != -1)
        return 15;
    return 0;
}

int main()
{
    int result = VerifySuccessfulInitialise();
    if (result == 0)
        result = VerifyRequestedDepthAndSmallDevice();
    if (result == 0)
        result = VerifyRejectedGameInitialise();

    if (result != 0)
    {
        printf("FABLETLC_GFINITIALISE_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_GFINITIALISE_BEHAVIOR PASS\n");
    return 0;
}
