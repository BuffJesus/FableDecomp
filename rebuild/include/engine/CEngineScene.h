#pragma once
#include <stddef.h>
#include "fable_base_class.h"
#include "fable_counted_pointer.h"
#include "fable_stlport_tree.h"
enum EEngineRenderLayer;
class CEngineSceneUpdateStats {
public:
    CEngineSceneUpdateStats() : StaticPrimitivesAdded(0),MovingPrimitivesAdded(0),NonPersistentPrimitives(0) {}
    unsigned long StaticPrimitivesAdded,MovingPrimitivesAdded,NonPersistentPrimitives;
};
class CEngineSceneRenderStats {
public:
    CEngineSceneRenderStats() : StaticPrimitivesRendered(0),MovingPrimitivesRendered(0),StaticPrimitivesTouched(0),MovingPrimitivesTouched(0),VisibleCells(0),BoundingBoxTests(0),BoundingSphereTests(0) {}
    unsigned long StaticPrimitivesRendered,MovingPrimitivesRendered,StaticPrimitivesTouched,MovingPrimitivesTouched,VisibleCells,BoundingBoxTests,BoundingSphereTests;
};
class CShadowClipVolume;
class CEngineCamera;
enum EEngineOcclusionLayer;
class CEngineSceneMap;
class CEngineInternalPrimitiveBase;
// Partial retail view: the nested maps use the shared 12-byte STLport tree.
// The unrecovered tail is not an allocation-size claim.
class CEngineScene : public CBaseClassNonCopyable {
public:
    typedef _STL::map<long,CCountedPointer<CEngineSceneMap> > MapColumn;
    typedef _STL::map<long,MapColumn> SceneMaps;
    void InvalidateBoundingBoxesAndDrawDistances();
    void FrameEnd();
    void RenderSceneShadowBuffer(EEngineRenderLayer,const CShadowClipVolume&);
    void RenderScene(EEngineRenderLayer,const CEngineCamera&);
    void RenderSceneAndCheckVisibility(EEngineRenderLayer,const CEngineCamera&,EEngineOcclusionLayer,bool);
    void ResetStats();
    static void FABLE_FASTCALL UpdatePosition(CEngineInternalPrimitiveBase&,unsigned long);
    void AddPrimitive(CEngineInternalPrimitiveBase&,unsigned long);
private:
    void UpdateMaps();
    void ClearNonPersistent();
public:
    SceneMaps MapList;
    float MapSize,OneOverMapSize;
    unsigned long MaxMapLevels,MapsRenderLayerMask;
    CEngineInternalPrimitiveBase *FirstMovingPrimitive,*FirstNonPersistentPrimitive;
    CCharString Name;
    _STL::map<EEngineRenderLayer,CEngineSceneRenderStats> RenderStats;
    CEngineSceneUpdateStats UpdateStats;
};
FABLE_STATIC_ASSERT(offsetof(CEngineScene,MapList)==4);

FABLE_STATIC_ASSERT(offsetof(CEngineScene,FirstNonPersistentPrimitive)==0x24);

FABLE_STATIC_ASSERT(offsetof(CEngineScene,RenderStats)==0x2c);
FABLE_STATIC_ASSERT(offsetof(CEngineScene,UpdateStats)==0x38);
