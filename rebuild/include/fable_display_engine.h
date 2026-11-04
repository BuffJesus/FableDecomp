#pragma once

// CDisplayEngine's polymorphic interface as retail dispatches through it.
//
// engine/CDisplayEngine.h is the generated PDB layout (a flat struct with no
// virtuals); a TU that needs CDisplayEngine's virtual destructor (e.g.
// boost::scoped_ptr<CDisplayEngine> in ~CMainGameComponent 0x00418B7A) or its
// engine pointer through a polymorphic CIEngine includes this header instead.
// The two must not meet in one TU.
//
// Retail RTTI: CDisplayEngine : CBaseClassNonCopyable. Vtable 0x01231574:
//   0 ??_G 0x00437070 (dtor 0x004354C0), 1 InitEngine 0x00435190,
//   2 UninitEngine 0x004354A0 (FableWin ?InitEngine@CDisplayEngine@@MAE_NXZ,
//   ?UninitEngine@CDisplayEngine@@MAEXXZ).
// Typed retail members retain Ego_r.pdb offsets and the 0x100-byte allocation.

#include <stddef.h>

#include "fable_base_class.h"
#include "rebuild_abi.h"

#include "fable_surface.h"
#include "fable_camera.h"
#include "fable_fade_in_out.h"
#include "fable_counted_pointer.h"

class CIEngine;
class CInterpolationInfo;
class CWorld;
class CMainGameComponent;
class CPlayerManager;
class CGameDefinitionManager;
class CGraphicDataBank;
class CMeshDataBank;
class CGameEvent;
class CGameEventPackage;
class CDisplayViewManager;
class CWideString;
extern CWideString GResetProgressTipText;       // retail013B89D8
extern CWideString GResetProgressTipTitleText;  // retail013B89DC
extern bool GInitialFadeAllowedToBeActive;     // retail01375CDC

// Ego_r.pdb CDisplayEngineInit (0x24). Its constructor is out of line in
// retail (0x00419270: zeroes every pointer/size, PixelFormat.FormatIndex = -1).
class CDisplayEngineInit
{
public:
    CDisplayEngineInit();

    CMainGameComponent* Component;                    // +0x00
    const CGameDefinitionManager* DefinitionManager;  // +0x04
    CPlayerManager* PlayerManager;                    // +0x08
    CWorld* World;                                    // +0x0C
    CGraphicDataBank* MainGraphicBank;                // +0x10
    unsigned long Width;                              // +0x14
    unsigned long Height;                             // +0x18
    CPixelFormat PixelFormat;                         // +0x1C
    CMeshDataBank* MeshBank;                          // +0x20
};

FABLE_STATIC_ASSERT(sizeof(CDisplayEngineInit) == 0x24);

class CDisplayEngine : public CBaseClassNonCopyable
{
public:
    // ??0CDisplayEngine@@QAE@ABVCDisplayEngineInit@@@Z (retail 0x00434E10;
    // retail operator new size 0x100).
    explicit CDisplayEngine(const CDisplayEngineInit& init);
    virtual ~CDisplayEngine();

    // ?Init@CDisplayEngine@@QAEXXZ (retail 0x004350D0) and
    // ?PreChangeResolution@CDisplayEngine@@QAEXXZ (retail 0x00434A10).
    void Init();
    void PreChangeResolution();
    // ?PostChangeResolution@CDisplayEngine@@QAEXKKK@Z (retail 0x00434A20).
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);

    // ?Render@CDisplayEngine@@QAEXPBVCInterpolationInfo@@_N1@Z (retail
    // 0x00435F70; CMainGameComponent::UpdateForcedTickSpeed 0x00417747 calls
    // it with (&info, WorldUpdateTurn, true)).
    void Render(const CInterpolationInfo* info, bool worldUpdated, bool present);

    // Ego_r.pdb names; retail addresses from CMainGameComponent's callers
    // (AddGameEvent 0x0041649C, UpdateFromEventPackageSet 0x0041726D,
    // ChangeTextureColourDepth 0x0041799B).
    void ReceiveFeedbackGameEvent(const CGameEvent* event);                    // 0x00434A30
    void ConfirmFeedbackGameEventPackage(const CGameEventPackage* package);    // 0x00434A60
    void WorldUpdate();                                                        // 0x00434F60
    void SetAboutToRun();
    void EndScreenFadeOut(); // retail 00434C90                                                       // 0x00434CD0
    static float GamePrimitiveDefaultFadeRangeRatio;                            // 0x013B8774
    void ChangeTextureColourDepth(long depth);                                 // 0x00434DC0

protected:
    // ?DoRender@CDisplayEngine@@IAEXPBVCInterpolationInfo@@_N1@Z (retail
    // 0x00435530; Render 0x00435F70 tail-jumps to it with the same arguments).
    void DoRender(const CInterpolationInfo* info, bool worldUpdated, bool present);
    void RenderHeroFuckShit(const CInterpolationInfo& info); // retail 00435000
    void RenderHeroDrunkenness(const CInterpolationInfo& info); // retail 00435070

    virtual bool InitEngine();
    virtual void UninitEngine();

public:
    unsigned long EnginePreloadFlags;           // +0x04
    CMainGameComponent* Component;              // +0x08
    CPlayerManager* PlayerManager;              // +0x0C
    CGameDefinitionManager* DefinitionManager;  // +0x10
    CGraphicDataBank* EngineGraphicBank;        // +0x14
    CMeshDataBank* MeshBank;                    // +0x18
    float MainWindow[4];                        // +0x1C C2DBoxF
    CIEngine* Engine3D;                         // +0x2C
    CCamera Camera;                             // +0x30
    long Frame;                                 // +0x68
    bool DrawGame;                              // +0x6C
    CLetterBoxModeInfo LetterBox;               // +0x70
    bool CameraInputsOn;                        // +0x90
    double LastRefreshTime;                     // +0x98
    double LastRenderTimeLength;                // +0xA0
    double LastWorldUpdateRenderTime;           // +0xA8
    bool PreparePrimitives;                     // +0xB0
    float GammaRamp,OldGammaRamp;                // +0xB4,+0xB8
    CFadeInFadeOutBase ScreenFadeOutInfo;         // +0xBC
    bool ScreenFadeOutLocked,DrawMemoryUse;      // +0xD8,+0xD9
    unsigned long DrawDebugPage;                 // +0xDC
    CCountedPointer<CDisplayViewManager> PViewManager; // +0xE0
    long InitialFadeReadynessCount;              // +0xE8 (PDB signed long)
    bool InitialFadeActive;                     // +0xEC
    float ScreenFadeDuration,TimeSinceFadeStarted; // +0xF0,+0xF4
    CWorld* World;                              // +0xF8
};

FABLE_STATIC_ASSERT(sizeof(CDisplayEngine) == 0x100);

FABLE_STATIC_ASSERT(offsetof(CDisplayEngine, Engine3D) == 0x2C);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine,Camera)==0x30);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine,LetterBox)==0x70);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine,LastRefreshTime)==0x98);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine,ScreenFadeOutInfo)==0xBC);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine,PViewManager)==0xE0);
FABLE_STATIC_ASSERT(offsetof(CDisplayEngine,World)==0xF8);
