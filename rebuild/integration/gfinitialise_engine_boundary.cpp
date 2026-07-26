#include "fable_boot.h"

#include <stdlib.h>
#include <string.h>

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

GFInitialiseProgressPhaseState g_GFInitialiseProgressPhaseState = {};
GFInitialiseState g_GFInitialiseState_013B83D0 = {};

fable_u32 g_FablePrimitiveManager_013B8394 = 0;
CDisplayManager* g_FableDisplayManager_013B8390 = 0;
FableDisplayPrimitive* g_FableDisplayPrimitive_013B8384 = 0;
void* g_FableRenderState_013B8380 = 0;
fable_u8 g_FableRenderStateStorage_013BC470 = 0;
fable_u32 g_FableRenderDevice_013B8388 = 0;
fable_u32 g_FableResourceManager_013B838C = 0;
fable_u32 g_FableTextureManager_013B837C = 0;
fable_u32 g_FableTextureBitDepth_013B7D68 = 0;
fable_u32 g_FableRequestedTextureBitDepth_01375470 = 32;
fable_u32 g_FableMaximumDisplayDimension_0137546C = 1280;
fable_u32 g_FableDisplayWidth_013B7D64 = 0;
fable_u32 g_FableDisplayHeight_013B7D60 = 0;
fable_i32 g_FableHalfDisplayWidth_0137544C = 0;
fable_u8 g_CProgressDisplayVTable_012388B4 = 0;
double g_FableZeroDouble_0122ED70 = 0.0;

bool g_FableProgressBeginEnabled = true;
fable_u32 g_FableProgressBeginCalls = 0;
fable_u32 g_FableProgressConstructCalls = 0;
fable_u32 g_FableProgressInstallCalls = 0;
fable_u32 g_FableProgressDestroyCalls = 0;

namespace
{
    FableEngineRoot g_FableBoundaryEngineRoot = {};
    FableDisplayPrimitive g_FableBoundaryDisplayPrimitive = {};
    CDisplayManager g_FableBoundaryDisplayManager = {};

    void FABLE_FASTCALL DestroyProgressDisplay(void* object)
    {
        if (object != 0)
        {
            CProgressDisplay* display =
                static_cast<CProgressDisplay*>(object);
            reinterpret_cast<CCharString*>(
                &display->secondaryText50)->~CCharString();
            reinterpret_cast<CCharString*>(
                &display->primaryText4C)->~CCharString();
            reinterpret_cast<CWideString*>(
                &display->wideText48)->~CWideString();
            ++g_FableProgressDestroyCalls;
            FableOperatorDelete(object);
        }
    }
}

FableEngineRoot* FABLE_FASTCALL AddChildPrimitive()
{
    return &g_FableBoundaryEngineRoot;
}

bool CDisplayManager::IsTextureBitDepthSupported(int bitDepth)
{
    return bitDepth == 32;
}

void CXMVPlayer::GlobalRegisterPlayer()
{
}

void FABLE_CDECL GFGetBuildNumber2()
{
}

void FABLE_FASTCALL FablePrepareGFInitialiseBoundary()
{
    memset(
        &g_FableBoundaryEngineRoot,
        0,
        sizeof(g_FableBoundaryEngineRoot));
    memset(
        &g_FableBoundaryDisplayPrimitive,
        0,
        sizeof(g_FableBoundaryDisplayPrimitive));
    memset(
        &g_FableBoundaryDisplayManager,
        0,
        sizeof(g_FableBoundaryDisplayManager));
    memset(
        &g_GFInitialiseState_013B83D0,
        0,
        sizeof(g_GFInitialiseState_013B83D0));

    g_FableBoundaryEngineRoot.renderDevice58 = 1;
    g_FableBoundaryEngineRoot.displayManager60 =
        &g_FableBoundaryDisplayManager;
    g_FableBoundaryEngineRoot.textureManager6C = 1;
    g_FableBoundaryEngineRoot.primitiveManager7C = 1;
    g_FableBoundaryEngineRoot.resourceManager84 = 1;

    g_FableBoundaryDisplayManager.displayPrimitive08 =
        &g_FableBoundaryDisplayPrimitive;
    g_FableBoundaryDisplayManager.displayWidth1C8 = 1280;
    g_FableBoundaryDisplayPrimitive.maximumWidth3AB0 = 1280;
    g_FableBoundaryDisplayPrimitive.maximumHeight3AB4 = 720;

    g_FableRequestedTextureBitDepth_01375470 = 32;
    g_FableMaximumDisplayDimension_0137546C = 1280;
    g_FableProgressBeginEnabled = true;
    g_FableProgressBeginCalls = 0;
    g_FableProgressConstructCalls = 0;
    g_FableProgressInstallCalls = 0;
    g_FableProgressDestroyCalls = 0;
}

void* FABLE_CDECL FableOperatorNew(fable_u32 size)
{
    if (size == sizeof(CProgressDisplay))
        ++g_FableProgressConstructCalls;
    return malloc(size);
}

void FABLE_CDECL FableOperatorDelete(void* allocation)
{
    free(allocation);
}

CCountedProgressDisplay::CCountedProgressDisplay(CProgressDisplay* value)
    : object(value),
      reference(0)
{
    if (value == 0)
        return;

    reference = static_cast<FableReferenceCount*>(
        FableOperatorNew(sizeof(FableReferenceCount)));
    if (reference == 0)
    {
        DestroyProgressDisplay(value);
        object = 0;
        return;
    }

    reference->owners = 1;
    reference->destroy = DestroyProgressDisplay;
    reference->object = value;
}

fable_u8 FABLE_FASTCALL GFInitialiseState_Begin(
    GFInitialiseState* /* state */)
{
    ++g_FableProgressBeginCalls;
    return g_FableProgressBeginEnabled ? 1 : 0;
}

void FABLE_FASTCALL SetProgressDisplay(
    CCountedProgressDisplay* /* display */)
{
    ++g_FableProgressInstallCalls;
}
