#pragma once
#include <stddef.h>
#include "fable_iengine.h"
#include "fable_device_reset_callback.h"
#include "fable_camera.h"
#include "fable_carray.h"
#include "fable_def_pointer.h"
#include "fable_simple_memory_pool.h"
#include "fable_extents_i.h"
#include "fable_extents_f.h"
#include "fable_display_viewport.h"
#include "fable_engine_video_option_quality.h"
class CEngineVideoOptions {
public:
    CEngineVideoOptions();
    C2DExtentsI ScreenResolution;
    C2DExtentsF ScreenAspectRatio;
    long BitsPerPixel,RefreshRate;
    bool VerticalSync;
    EEngineVideoOptionQuality LandscapeDetail,MeshDetail,TextureDetail,ShadowQuality,EffectsQuality;
    ESurfaceMultisampleType Antialiasing;
};
class CEngineLayerRenderer;
class CEngineComponent;
class CDefClassBase;

// Native retail allocation is0x178bytes. Ego_r's0x188-byte layout uses16-byte
// CArrays; retail uses12-byte vectors. PreloadStats retains8-byte alignment.
// Primary86-slot and device2-slot tables are audited against donor PDB
// identities and retail targets in native_engine_vtables.json. Base CIEngine
// contributes85 audited slots; inherited slots1,17,18,50,51 are preserved.
// Do not combine with the generated flat engine/CEngine.h in one TU.
class CEngine : public CIEngine,public CDeviceResetCallback {
public:
    CEngine(); // retail00B260B0
    virtual ~CEngine();
    virtual bool Initialise(const CEngineInit& init);
    virtual void Release();
    virtual void SetCamera(const CCamera* camera);
    virtual void SetWindow(const C2DBoxF* window);
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool always);
    virtual void SetWorld(const CIDrawWorld* world);
    virtual void Render(const CEngineInterpolationInfo& info, unsigned long flags);
    virtual void WorldFrameUpdate(bool firstFrame);
    virtual void PausedWorldFrameUpdate();
    virtual bool ConvertScreenPosToWorldPos(const C2DVector& screen, C3DVector& world) const;
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screen, C3DVector& world) const;
    virtual bool ConvertWorldPosToScreenPos(const C3DVector& world, C2DVector& screen) const;
    virtual bool SetVideoOptions(const CEngineVideoOptions& options);
    virtual bool PreChangeResolution();
    virtual bool PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);
    virtual float GetPrimitiveDrawDistanceFactor() const;
    virtual void AddPrimitive(const CEnginePrimitive& primitive);
    virtual bool ClipSphere(const C3DBoundingSphere& sphere) const;
    virtual bool ClipBox(const C3DBoundingBox& box) const;
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle& handle, const CEnginePrimitiveAnimation& anim);
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle& handle, const CEnginePrimitivePosition& pos);
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);
    virtual void SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
    virtual void SetChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long index, CEnginePrimitiveHandle& child, const CEnginePrimitive& primitive, unsigned long flags);
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long index, CEnginePrimitiveHandle& child);
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle& handle, unsigned long index);
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle& handle);
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& info, const CEnginePrimitiveHandle& parent, long boneIndex, unsigned long flags);
    virtual void CollectPrimitiveStatsForArea(CIEnginePrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex_t index, unsigned long flags) const;
    virtual void ProcessPrimitiveStats(const CIEnginePrimitiveStatMap& stats, EEnginePrimitiveStatIndex_t index, unsigned long& total, CArray<CCharString>& lines) const;
    virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString& name) const;
    virtual unsigned long AddCustomMorphTexture(CTexture& texture, const CCharString& name) const;
    virtual long GetStaticMapUpdatePassCount();
    virtual void StartStaticMapUpdatePass(const CWideString& file, long pass, const CStaticMapGenerationVisibiilityInfo& info, const CArray<CCharString>& names);
    virtual bool IsStaticMapNeedingUpdate(long map, const CCharString& name, unsigned long checksum) const;
    virtual void UpdateStaticMap(long map, const CCharString& name, unsigned long checksum, long pass);
    virtual void CloseStaticMapFile();
    virtual void AddMap(long map, long level);
    virtual void RemoveMap(long map);
    virtual void SetMapAsLoaded(long map);
    virtual void SetMapAsUnloaded(long map);
    virtual void SetMapVisibilityAllMaps(bool visible);
    virtual void SetMapVisibility(long map, bool visible);
    virtual void AreaChanged(const C2DBoxI& area);
    virtual void OnActivate();
    virtual void SetStaticMapFileForUse(const CWideString& file);
    virtual void SetLandscapeDebugTintColour(long map, CRGBColour colour);
    virtual void ReloadLocalDetailDefs();
    virtual void StartNewScene(unsigned long flags, float time);
    virtual void CutscenePreload(bool start, bool wait);
    virtual void CutsceneAddCameraAngle(const CCamera& camera);
    virtual void ChangeCameraAngle();
    virtual bool IsPreloaded() const;
    virtual void UpdateTextureMorphCache(const CArray<CEngineTextureMorph>& morphs, bool force, float time, const CIEngineLongSet* used);
    virtual void SetScreenColourFilter(const CEngineColourFilterSettings& s);
    virtual void SetSpotlightShadowMode(bool on);
    virtual void SetSeaBodyToRender(CCharString name);
    virtual void UpdateDebugOverlay(long overlay, const CRGBColour* colours, const C2DBoxI& area);
    virtual void EnableDebugOverlay(long overlay, bool enable, bool redraw);
    virtual void SetGlobalLightingParameters(const C3DVector& direction, const CRGBFloatColour& ambient, const CRGBFloatColour& diffuse, const CRGBFloatColour& backlight);
    virtual void Set2DPrimitiveLightingParameters(const C3DVector& direction, const CRGBFloatColour& ambient, const CRGBFloatColour& diffuse, const CRGBFloatColour& backlight);
    virtual void SetRenderTargetTexture(const CTexture* texture);
    virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long flags, const C2DExtentsI& size, long mips, const CPixelFormat& format);
    virtual void FreeTextureFromEngineResourcePools(CTexture* texture);
    virtual CMovableResourceMemoryPool* GetLoadingMemoryPool();
    virtual CMovableResourceMemoryPool* GetAnimationMemoryPool();
    virtual CGPUScratchBufferTextureManager* GetScratchBufferTextureManager();
    virtual bool DeallocateShadowBuffer();
    virtual void ReallocateShadowBuffer();
    virtual void Enable3DSceneRendering(bool enable);
    virtual bool Is3DSceneRenderingEnabled();
    virtual void Enable2DSceneRendering(bool enable);
    virtual bool Is2DSceneRenderingEnabled();
    virtual void OverrideRain(float amount);
    virtual void OverrideSnow(float amount);
    virtual void RenderColorFilteredQuad(float x, float y, const C4DVector& colour, float width, float height, CTexture* texture);
    virtual const CRGBFloatColour PeekGlobalDiffuseColour() const;
    virtual const CRGBFloatColour PeekGlobalAmbientColour() const;
    virtual void Uninitialise();
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
private:
    void UpdateOffGameTickProcessInfo(bool);
    void LoadNewScene(const CEngineInterpolationInfo&);
    void RenderFrame(const CEngineInterpolationInfo&);
    void SetDefaultRenderStates();
    enum ESmartRestoreComponentVideoMemoryResult {
        SMART_RESTORE_COMPONENT_VIDEO_MEMORY_SUCCESS=0,
        SMART_RESTORE_COMPONENT_VIDEO_MEMORY_REDUCED_DETAIL=1,
        SMART_RESTORE_COMPONENT_VIDEO_MEMORY_FAILED=2
    };
    ESmartRestoreComponentVideoMemoryResult SmartRestoreComponentVideoMemory();
public:
    struct CPreloadStats {
        CPreloadStats();
        double LastFullPreloadTime;
        bool LastFullPreloadTimedOut;
        double LastFastPreloadTime;
        bool LastFastPreloadTimedOut;
    };
    struct COffWorldTickProcessInfo {
        bool ProcessThisFrame,ProcessingEnabled;
        float ProcessTicksBehindAccumilator,MaxProcessTicksBehind;
        float MaxProcessTicksBehindBeforeCatchup,MinProcessFrequency;
        double LastFrameTimeStamp;
    };
    bool EnableWireFrame,EnableEngine,EnableLocalLights,EnableTextures;
    CCamera* CurrentCamera;
    bool NewSceneFlag;
    unsigned long NewSceneLoadFlags;
    float NewSceneTimeOut;
    bool CutscenePreloadFlag;
    float CutscenePreloadTimeout,CutsceneFastPreloadTimeout,ActorAdditionalLoadTimeout,ActorRadius;
    bool ActorsLoading,CameraAngleChanged;
    CArrayVector<CCamera> PreloadCameras;
    CPreloadStats PreloadStats;
    C3DVector PreloadedCameraPos,PreloadedLookVector;
    float StartNewSceneTransitionTime;
    bool IsInitialised;
    long CurrentRenderFrame,CurrentWorldFrame,CurrentPausedWorldFrame;
    double NonInterpolatedWorldTime,NonInterpolatedPausedWorldTime,InterpolatedWorldTime;
    unsigned long WorldFramesPerSecond;
    bool BlockOnLoad;
    unsigned long SceneFilterFlags;
    bool EnableScreenShotMode,EnablePostDeviceReset;
    CEngineVideoOptions VideoOptions;
    COffWorldTickProcessInfo OffWorldTickProcessInfo;
    CDefPointer<const CDefClassBase> PEnvironmentDef,PEngineDef,PEngineVideoOptionsDefs[4];
    CSimpleMemoryPool FrameMemoryPool;
    CArrayVector<CSurface*> ScratchZBuffersD24S8;
    long AnisotropicFilteringLevel;
    CArrayVector<CEngineLayerRenderer*> RenderLayers;
    CArrayVector<CEngineComponent*> EngineComponents;
};
FABLE_STATIC_ASSERT(sizeof(CEngineVideoOptions)==0x34);
FABLE_STATIC_ASSERT(sizeof(CEngine::CPreloadStats)==0x20);
FABLE_STATIC_ASSERT(sizeof(CEngine::COffWorldTickProcessInfo)==0x20);
FABLE_STATIC_ASSERT(sizeof(CEngine)==0x178);
FABLE_STATIC_ASSERT(offsetof(CEngine,PreloadStats)==0x48);
FABLE_STATIC_ASSERT(offsetof(CEngine,FrameMemoryPool)==0x130);
FABLE_STATIC_ASSERT(offsetof(CEngine,EngineComponents)==0x168);
