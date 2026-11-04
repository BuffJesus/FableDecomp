#include <new>
#include "fable_boot.h"
#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"

#include <stdlib.h>
#include <string.h>

struct CXMVPlayer
{
    static void GlobalRegisterPlayer();
};

GFInitialiseProgressPhaseState g_GFInitialiseProgressPhaseState = {};

CASoundManager* g_FableSoundManager_013B8394 = 0;
CDisplayManager* g_FableDisplayManager_013B8390 = 0;
CRenderManager2D* g_FableRenderManager2D_013B8384 = 0;
void* g_FableRenderState_013B8380 = 0;
fable_u8 g_FableRenderStateStorage_013BC470 = 0;
CInputManager* g_FableInputManager_013B8388 = 0;
CFontManager* g_FableFontManager_013B838C = 0;
CGraphicBankManager* g_FableGraphicBankManager_013B837C = 0;
unsigned long g_FableTextureBitDepth_013B7D68 = 0;
unsigned long g_FableRequestedTextureBitDepth_01375470 = 32;
unsigned long g_FableMaximumTextureSize_0137546C = 1280;
unsigned long g_FableMaxTextureWidth_013B7D64 = 0;
unsigned long g_FableMaxTextureHeight_013B7D60 = 0;
long g_FableHalfRefreshRate_0137544C = 0;
fable_u8 g_CProgressDisplayVTable_012388B4 = 0;
double g_FableZeroDouble_0122ED70 = 0.0;

bool g_FableProgressBeginEnabled = true;
fable_u32 g_FableProgressBeginCalls = 0;
fable_u32 g_FableProgressConstructCalls = 0;
fable_u32 g_FableProgressInstallCalls = 0;
fable_u32 g_FableProgressDestroyCalls = 0;

namespace
{
    // Stand-ins for the engine objects GFInitialise reads through the real
    // CSystemManager singleton: a display manager, its 2D render manager, and
    // one distinct address for each of the other cached managers.
    union
    {
        double align;
        unsigned char bytes[sizeof(CDisplayManager)];
    } g_FableBoundaryDisplayManager;
    union
    {
        double align;
        unsigned char bytes[sizeof(CRenderManager2D)];
    } g_FableBoundaryRenderManager;
    unsigned char g_FableBoundarySoundManager;
    unsigned char g_FableBoundaryInputManager;
    unsigned char g_FableBoundaryFontManager;
    unsigned char g_FableBoundaryGraphicBankManager;

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

bool CDisplayManager::IsTextureBitDepthSupported(unsigned long bitDepth)
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
    FableReleaseProgressDisplayBoundary();

    memset(
        g_FableBoundaryDisplayManager.bytes,
        0,
        sizeof(g_FableBoundaryDisplayManager.bytes));
    memset(
        g_FableBoundaryRenderManager.bytes,
        0,
        sizeof(g_FableBoundaryRenderManager.bytes));

    CDisplayManager* const displayManager =
        reinterpret_cast<CDisplayManager*>(g_FableBoundaryDisplayManager.bytes);
    CRenderManager2D* const renderManager =
        reinterpret_cast<CRenderManager2D*>(g_FableBoundaryRenderManager.bytes);
    new (&displayManager->PRenderManager) boost::scoped_ptr<CRenderManager>(reinterpret_cast<CRenderManager*>(renderManager));
    displayManager->RefreshRate = 60;
    renderManager->D3DDeviceCaps.MaxTextureWidth = 1280;
    renderManager->D3DDeviceCaps.MaxTextureHeight = 720;

    // Install them in the singleton GFInitialise reads (0x013CA618).
    CSystemManager* const systemManager = GFGetSystemManager();
    systemManager->PSoundManager.Data =
        reinterpret_cast<CASoundManager*>(&g_FableBoundarySoundManager);
    systemManager->PDisplayManager.Data = displayManager;
    systemManager->PInputManager.Data =
        reinterpret_cast<CInputManager*>(&g_FableBoundaryInputManager);
    systemManager->PFontManager.Data =
        reinterpret_cast<CFontManager*>(&g_FableBoundaryFontManager);
    systemManager->PGraphicBankManager.Data =
        reinterpret_cast<CGraphicBankManager*>(&g_FableBoundaryGraphicBankManager);

    g_FableRequestedTextureBitDepth_01375470 = 32;
    g_FableMaximumTextureSize_0137546C = 1280;
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

void FABLE_FASTCALL FableReleaseProgressDisplayBoundary()
{
    CCountedProgressDisplay emptyDisplay(0);
    SetProgressDisplay(&emptyDisplay);
}
