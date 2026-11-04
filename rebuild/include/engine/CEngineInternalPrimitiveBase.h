#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
// Canonical persistent/internal primitive hierarchy, reconciled with Ego_r.pdb
// and retail SetPrimitiveAnimation/Position virtual slots23/24 and fields.
// Existing retail data member names/offsets are preserved.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_engine_primitive_handle.h"

class CEngineSceneGridCell;
class CEnginePrimitive;
class CEnginePrimitiveAnimation;
class CEnginePrimitivePosition;
class CEngineMeshEffect;
class CEngineInternalPrimitiveOcclusionBase;
class CEngineFadeDistance;
class CEngineCamera;
class C3DVector;
class CMatrix3x4;
class C3DBoundingBox;
class C3DBoundingSphere;

#pragma pack(push, 1)
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    // Original virtual order from Ego_r.pdb: inherited slots 0..4, then 5..27.
    virtual void Update(const CEnginePrimitive&,unsigned long);
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive&,unsigned long);
    // VC7.1 reverses overloads within the virtual group: handle overload is slot7.
    virtual void SetChildPrimitive(unsigned long,const CEnginePrimitive&,unsigned long);
    virtual void SetChildPrimitive(unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
    virtual void GetChildPrimitiveHandle(unsigned long,CEnginePrimitiveHandle&);
    virtual void ClearChildPrimitive(unsigned long);
    virtual void ClearAllChildPrimitives();
    virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long);
    virtual CEngineFadeDistance GetFadeDistance() const;
    virtual void PostRender();
    virtual bool GetWorldPosition(C3DVector&);
    virtual bool GetBoundingBoxTransform(CMatrix3x4&);
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox&);
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere&);
    virtual bool GetBoundingSphereRadiusWorldSpace(float&);
    virtual bool IsInsideFrustum(const CEngineCamera&,bool);
    virtual bool GetObjectTransform(CMatrix3x4&) const;
    virtual bool SetObjectTransform(const CMatrix3x4&);
    virtual void SetAnimation(const CEnginePrimitiveAnimation&);
    virtual void SetPosition(const CEnginePrimitivePosition&);
    virtual void SetMeshEffect(const CIVCountedPointer<CEngineMeshEffect>&);
    virtual bool GetMeshEffect(CIVCountedPointer<CEngineMeshEffect>&) const;
    virtual CEngineInternalPrimitiveOcclusionBase* GetAsOcclusionPrimitive();
    void SetFadeDistanceWithGamePrimitiveDefaults(float);
    void AddToList(CEngineInternalPrimitiveBase**);
    void RemoveFromList();
    unsigned long                 EnginePrimitiveFlags;      // +0x0c
    float                         AlphaMipBias;              // +0x10
    float                         FadeDistance_FadeBeginDistance; // +0x14 CEngineFadeDistance::FadeBeginDistance
    float                         FadeDistance_MaxDrawDistance; // +0x18 CEngineFadeDistance::MaxDrawDistance
    unsigned long                 OcclusionLayerBitMask;     // +0x1c
    CEngineSceneGridCell*         GridCell;                  // +0x20
    signed char                   GridPos_Level;             // +0x24 CEngineSceneGridPos::Level
    unsigned char                 GridPos_X;                 // +0x25 CEngineSceneGridPos::X
    unsigned char                 GridPos_Y;                 // +0x26 CEngineSceneGridPos::Y
    unsigned char                 Alpha;                     // +0x27
    unsigned char                 AlphaRef;                  // +0x28
    unsigned char                 PreloadStat;               // +0x29
    unsigned char                 _pad_0x2a[0x2];            // +0x2a
    unsigned char                 InterpolationSet[0x4];     // +0x2c EEngineInterpolationSet
    unsigned long                 RenderLayerMask;           // +0x30
    union {
        unsigned char PostRenderEnable[2];
        struct { unsigned char Enable:1; unsigned char NonPersistent:1; unsigned char RemainingFlags:6; unsigned char FlagsPadding; };
    };
    unsigned short                LastFrameRendered;         // +0x36
    CEngineInternalPrimitiveBase* NextPrimitive;             // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;             // +0x3c
    CEngineInternalPrimitiveBase* NextLayerMask;             // +0x40
    CEngineInternalPrimitiveBase** RefLayerMask;             // +0x44
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveBase) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, EnginePrimitiveFlags) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, AlphaMipBias) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, FadeDistance_FadeBeginDistance) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, FadeDistance_MaxDrawDistance) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, OcclusionLayerBitMask) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, GridCell) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, GridPos_Level) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, GridPos_X) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, GridPos_Y) == 0x26);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, Alpha) == 0x27);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, AlphaRef) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, PreloadStat) == 0x29);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, InterpolationSet) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, RenderLayerMask) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, PostRenderEnable) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, LastFrameRendered) == 0x36);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, NextPrimitive) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, RefPrimitive) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, NextLayerMask) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase, RefLayerMask) == 0x44);

#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
