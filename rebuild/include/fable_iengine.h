#pragma once

// CIEngine -- the polymorphic 3D engine interface (FableWin.pdb `CIEngine`,
// Ego_r.pdb sizeof 0xC: CInitBaseClass {vptr, Valid} + bool Active at +0x08).
//
// DO NOT include this header in the same translation unit as the generated
// flat layout "engine/CIEngine.h": that header declares `struct CIEngine` as a
// pad struct with no virtuals, and the two definitions clash. Use this header
// whenever code calls a CIEngine virtual (e.g. PDisplayEngine->Engine3D->...).
// A bare forward declaration (`class CIEngine;` / `struct CIEngine;`) is
// compatible with either.
//
// Slot order is RETAIL, recovered via RTTI (tools/rtti_map.py CIEngine / CEngine):
//   retail ??_7CIEngine@@6B@ = 0x012A0D24 (85 slots), concrete
//   retail ??_7CEngine@@6B@  = 0x012A0F3C (86 slots: these 85 + CEngine::Uninitialise).
// Every slot was matched against the FableWin donor vtable (0x0416965C) by the
// `ret N` argument-byte count of retail's CIEngine default body (pure slots
// 4,5,8,11,12,13,15,16,23 checked against retail CEngine's bodies instead);
// all 85 agree except slot 63, where retail's CCharString by value is 4 bytes
// (donor 8). No slot is unknown, so there are no UnknownSlotNN placeholders.
// Pure-virtual slots are _purecall (0x00BFEA08) in retail's CIEngine vtable.
//
// Signatures use the FableWin names/types. Parameter types that are STLport
// templates (map/set) are declared through opaque tag classes: every such
// parameter is a reference/pointer, so the ABI (and slot/ret-N) is identical.
// CIEngine::Uninitialise is NON-virtual in the PDB (?Uninitialise@CIEngine@@QAEXXZ).
//
// Owner: CMainGameComponent::UpdateRegular lane (2026-09-25). Keep edits additive.

#include "fable_init_base_class.h"

class CCamera;
class C2DBoxF;
class C2DBoxI;
class C2DExtentsI;
class C2DVector;
class C3DVector;
class C4DVector;
class C3DAnimationInfo;
class C3DBoundingBox;
class C3DBoundingSphere;
class CCharString;
class CEngineColourFilterSettings;
class CEngineInit;
class CEngineInterpolationInfo;
class CEnginePrimitive;
class CEnginePrimitiveAnimation;
class CEnginePrimitiveAttachmentInfo;
class CEnginePrimitiveHandle;
class CEnginePrimitivePosition;
class CEngineTextureMorph;
class CEngineVideoOptions;
class CGPUScratchBufferTextureManager;
class CIDrawWorld;
class CMovableResourceMemoryPool;
class CPixelFormat;
class CRGBColour;
class CRGBFloatColour;
class CStaticMapGenerationVisibiilityInfo;
class CTexture;
class CWideString;
template<typename T> class CArray;

// Opaque stand-ins for STLport containers passed by reference/pointer:
//   CIEnginePrimitiveStatMap = _STL::map<unsigned long, unsigned long>
//   CIEngineLongSet          = _STL::set<long>
class CIEnginePrimitiveStatMap;
class CIEngineLongSet;

// FableWin W4EEnginePrimitiveStatIndex@@ (passed by value, 4 bytes).
typedef long EEnginePrimitiveStatIndex_t;

class CIEngine : public CInitBaseClass
{
public:
    CIEngine() : Active(false) {}
    virtual ~CIEngine();                                                     // [ 0] +0x000 0x00B23770 (??_E)
    // [ 1] +0x004 CInitBaseClass::Validate 0x0099A340 (inherited)
    virtual bool Initialise(const CEngineInit& init);                        // [ 2] +0x008 0x00B232D0
    virtual void Release();                                                  // [ 3] +0x00C 0x00B232E0
    virtual void SetCamera(const CCamera* camera) = 0;                       // [ 4] +0x010
    virtual void SetWindow(const C2DBoxF* window) = 0;                       // [ 5] +0x014
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool always);           // [ 6] +0x018
    virtual void SetWorld(const CIDrawWorld* world);                         // [ 7] +0x01C
    virtual void Render(const CEngineInterpolationInfo& info,
                        unsigned long flags) = 0;                            // [ 8] +0x020
    virtual void WorldFrameUpdate(bool firstFrame);                          // [ 9] +0x024
    virtual void PausedWorldFrameUpdate();                                   // [10] +0x028
    virtual bool ConvertScreenPosToWorldPos(const C2DVector& screen,
                                            C3DVector& world) const = 0;     // [11] +0x02C
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float distance,
                                            const C2DVector& screen,
                                            C3DVector& world) const = 0;     // [12] +0x030
    virtual bool ConvertWorldPosToScreenPos(const C3DVector& world,
                                            C2DVector& screen) const = 0;    // [13] +0x034
    virtual bool SetVideoOptions(const CEngineVideoOptions& options);        // [14] +0x038
    virtual bool PreChangeResolution() = 0;                                  // [15] +0x03C
    virtual bool PostChangeResolution(unsigned long width,
                                      unsigned long height,
                                      unsigned long depth) = 0;              // [16] +0x040
    virtual void Refresh();                                                  // [17] +0x044
    virtual bool CheckPrimitiveVisibility(unsigned __int64 id);              // [18] +0x048
    virtual float GetPrimitiveDrawDistanceFactor() const;                    // [19] +0x04C
    virtual void AddPrimitive(const CEnginePrimitive& primitive);            // [20] +0x050
    virtual bool ClipSphere(const C3DBoundingSphere& sphere) const;          // [21] +0x054
    virtual bool ClipBox(const C3DBoundingBox& box) const;                   // [22] +0x058
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle,
                                        const CEnginePrimitive& primitive,
                                        unsigned long flags) = 0;            // [23] +0x05C
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle& handle,
                                       const CEnginePrimitiveAnimation& anim); // [24] +0x060
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle& handle,
                                      const CEnginePrimitivePosition& pos);  // [25] +0x064
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle& handle,
                                            const CEnginePrimitive& primitive,
                                            unsigned long flags);            // [26] +0x068
    // MSVC lays overloaded virtuals out in REVERSE declaration order: the
    // 4-argument overload is declared first so the 5-argument one lands in
    // slot 27 and the 4-argument one in slot 28, as retail has them.
    virtual void SetChildPrimitive(CEnginePrimitiveHandle& handle,
                                   unsigned long index,
                                   const CEnginePrimitive& primitive,
                                   unsigned long flags);                     // [28] +0x070 0x00B233D0 (ret 16)
    virtual void SetChildPrimitive(CEnginePrimitiveHandle& handle,
                                   unsigned long index,
                                   CEnginePrimitiveHandle& child,
                                   const CEnginePrimitive& primitive,
                                   unsigned long flags);                     // [27] +0x06C 0x00B233E0 (ret 20)
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle,
                                         unsigned long index,
                                         CEnginePrimitiveHandle& child);     // [29] +0x074
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle& handle,
                                     unsigned long index);                   // [30] +0x078
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle& handle);    // [31] +0x07C
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle& handle,
                                      const CEnginePrimitive& primitive,
                                      const CEnginePrimitiveAttachmentInfo& info,
                                      const CEnginePrimitiveHandle& parent,
                                      long boneIndex,
                                      unsigned long flags);                  // [32] +0x080
    virtual void CollectPrimitiveStatsForArea(CIEnginePrimitiveStatMap& stats,
                                              const C2DBoxF& area,
                                              EEnginePrimitiveStatIndex_t index,
                                              unsigned long flags) const;    // [33] +0x084
    virtual void ProcessPrimitiveStats(const CIEnginePrimitiveStatMap& stats,
                                       EEnginePrimitiveStatIndex_t index,
                                       unsigned long& total,
                                       CArray<CCharString>& lines) const;    // [34] +0x088
    virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString& name) const; // [35] +0x08C
    virtual unsigned long AddCustomMorphTexture(CTexture& texture,
                                                const CCharString& name) const; // [36] +0x090
    virtual long GetStaticMapUpdatePassCount();                              // [37] +0x094
    virtual void StartStaticMapUpdatePass(const CWideString& file,
                                          long pass,
                                          const CStaticMapGenerationVisibiilityInfo& info,
                                          const CArray<CCharString>& names); // [38] +0x098
    virtual bool IsStaticMapNeedingUpdate(long map,
                                          const CCharString& name,
                                          unsigned long checksum) const;     // [39] +0x09C
    virtual void UpdateStaticMap(long map,
                                 const CCharString& name,
                                 unsigned long checksum,
                                 long pass);                                 // [40] +0x0A0
    virtual void CloseStaticMapFile();                                       // [41] +0x0A4
    virtual void AddMap(long map, long level);                               // [42] +0x0A8
    virtual void RemoveMap(long map);                                        // [43] +0x0AC
    virtual void SetMapAsLoaded(long map);                                   // [44] +0x0B0
    virtual void SetMapAsUnloaded(long map);                                 // [45] +0x0B4
    virtual void SetMapVisibilityAllMaps(bool visible);                      // [46] +0x0B8
    virtual void SetMapVisibility(long map, bool visible);                   // [47] +0x0BC
    virtual void AreaChanged(const C2DBoxI& area);                           // [48] +0x0C0
    virtual void OnActivate();                                               // [49] +0x0C4
    virtual void OnDeactivate();                                             // [50] +0x0C8
    virtual bool IsActive() const;                                           // [51] +0x0CC
    virtual void SetStaticMapFileForUse(const CWideString& file);            // [52] +0x0D0
    virtual void SetLandscapeDebugTintColour(long map, CRGBColour colour);   // [53] +0x0D4
    virtual void ReloadLocalDetailDefs();                                    // [54] +0x0D8
    virtual void StartNewScene(unsigned long flags, float time);             // [55] +0x0DC
    virtual void CutscenePreload(bool start, bool wait);                     // [56] +0x0E0
    virtual void CutsceneAddCameraAngle(const CCamera& camera);              // [57] +0x0E4
    virtual void ChangeCameraAngle();                                        // [58] +0x0E8
    virtual bool IsPreloaded() const;                                        // [59] +0x0EC (CIEngine default: return true)
    virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph>& morphs,
                                         bool force,
                                         float time,
                                         const CIEngineLongSet* used);       // [60] +0x0F0
    virtual void SetScreenColourFilter(const CEngineColourFilterSettings& s); // [61] +0x0F4
    virtual void SetSpotlightShadowMode(bool on);                            // [62] +0x0F8
    virtual void SetSeaBodyToRender(CCharString name);                       // [63] +0x0FC (retail ret 4)
    virtual void UpdateDebugOverlay(long overlay,
                                    const CRGBColour* colours,
                                    const C2DBoxI& area);                    // [64] +0x100
    virtual void EnableDebugOverlay(long overlay, bool enable, bool redraw); // [65] +0x104
    virtual void SetGlobalLightingParameters(const C3DVector& direction,
                                             const CRGBFloatColour& ambient,
                                             const CRGBFloatColour& diffuse,
                                             const CRGBFloatColour& backlight); // [66] +0x108
    virtual void Set2DPrimitiveLightingParameters(const C3DVector& direction,
                                                  const CRGBFloatColour& ambient,
                                                  const CRGBFloatColour& diffuse,
                                                  const CRGBFloatColour& backlight); // [67] +0x10C
    virtual void SetRenderTargetTexture(const CTexture* texture);            // [68] +0x110
    virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long flags,
                                                             const C2DExtentsI& size,
                                                             long mips,
                                                             const CPixelFormat& format); // [69] +0x114
    virtual void FreeTextureFromEngineResourcePools(CTexture* texture);      // [70] +0x118
    virtual CMovableResourceMemoryPool* GetLoadingMemoryPool();              // [71] +0x11C
    virtual CMovableResourceMemoryPool* GetAnimationMemoryPool();            // [72] +0x120
    virtual CGPUScratchBufferTextureManager* GetScratchBufferTextureManager(); // [73] +0x124
    virtual bool DeallocateShadowBuffer();                                   // [74] +0x128
    virtual void ReallocateShadowBuffer();                                   // [75] +0x12C
    virtual void Enable3DSceneRendering(bool enable);                        // [76] +0x130
    virtual bool Is3DSceneRenderingEnabled();                                // [77] +0x134
    virtual void Enable2DSceneRendering(bool enable);                        // [78] +0x138
    virtual bool Is2DSceneRenderingEnabled();                                // [79] +0x13C
    virtual void OverrideRain(float amount);                                 // [80] +0x140
    virtual void OverrideSnow(float amount);                                 // [81] +0x144
    virtual void RenderColorFilteredQuad(float x,
                                         float y,
                                         const C4DVector& colour,
                                         float width,
                                         float height,
                                         CTexture* texture);                 // [82] +0x148
    virtual const CRGBFloatColour PeekGlobalDiffuseColour() const;           // [83] +0x14C
    virtual const CRGBFloatColour PeekGlobalAmbientColour() const;           // [84] +0x150

    void Uninitialise();                                                     // non-virtual

protected:
    bool Active;                                                             // +0x08
};

FABLE_STATIC_ASSERT(sizeof(CIEngine) == 0x0C);
