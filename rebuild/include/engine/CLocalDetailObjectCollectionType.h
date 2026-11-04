#pragma once
#include "fable_base_class.h"
class C3DMeshInfo;
class CDataInputStream;
class CCharString;
// Ego_r layout and retail BDEDD0 stack temporary: 0x54 bytes.
class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable
{
public:
    CLocalDetailObjectCollectionType() : Mesh(0),ZSpriteMesh(0),ShadowMesh(0) {}
    CLocalDetailObjectCollectionType(const CLocalDetailObjectCollectionType&);
    bool IsEquivalent(const CLocalDetailObjectCollectionType&) const;
    void Load(CDataInputStream&,bool);
    const C3DMeshInfo *Mesh,*ZSpriteMesh,*ShadowMesh;
    long MeshPolyCount,MeshBankIndex,ZSpriteMeshBankIndex,ShadowMeshBankIndex;
    struct {
        float FadeStart,FadeEnd;
        long AlphaRef;
        float AlphaMipBias,WindSkewConstantFactor,WindSkewRandomFactor,WindSkewSpeedFactor;
        float ZSpriteFadeStart,ZSpriteFadeEnd;
        int PrimitiveType;
        long CacheGroup;
        bool AlphaIsBoolean,LandscapeNormalLighting,HasWindSkew,CastsShadows,ReceivesShadows;
    } PlainDataBlock;
private:
    long FindMeshByName(CCharString) const;
};
FABLE_STATIC_ASSERT(sizeof(CLocalDetailObjectCollectionType)==0x54);
