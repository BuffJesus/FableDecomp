#pragma once
#include <stddef.h>
#include "fable_base_class.h"
#include "fable_carray.h"
#include "fable_vertex_shader_constant.h"

class CCharString;
class CShaderConstantRange : public CBaseClass
{
public:
    CShaderConstantRange():Offset(0),Count(0){}
    bool operator!=(const CShaderConstantRange& other) const;
    long Offset;
    unsigned long Count;
};

// Ego_r names; the fixed range fields agree with retail 0098B5E0 and
// 00989760/009897C0/0098A760. The preset vector and range lifetimes are reconstructed.
class CVertexShaderConstantLayout : public CBaseClass
{
public:
    CVertexShaderConstantLayout();
    virtual CCharString PeekName();
    CShaderConstantRange Reserved, User, Compression, Ambient, FogColour, CameraPos;
    unsigned long LightSize;
    CShaderConstantRange LightArray, LightGlobals, LightAttenuations;
    CShaderConstantRange CombinedProjectionMatrix, FogTransform, WorldMatrix;
    CShaderConstantRange SphereMapMatrix, TextureTransform[4], BoneMatrices;
    CShaderConstantRange ShadowTextureTransform, ShadowFadeTransform, ShadowFadeFactors;
    CShaderConstantRange ShadowedSpotlightFalloffTransform, ShadowedSpotlightAttenuation;
    CShaderConstantRange ShadowedSpotlightColour, ScreenSpaceTransformScale, ScreenSpaceTransformOffset;
    CArrayVector<CVertexShaderConstant> ReservedPresetsData;
    const CArrayVector<CVertexShaderConstant>& ReservedPresets() const {return ReservedPresetsData;}

};
FABLE_STATIC_ASSERT(sizeof(CShaderConstantRange)==12);
FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstantLayout)==0x14C);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout,Ambient)==0x28);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout,FogColour)==0x34);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout,LightGlobals)==0x5C);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout,ReservedPresetsData)==0x140);
// Retail RTTI at vtable0129B9B0; CRT initializer01224140 constructs this type.
class CVertexShaderConstantLayoutEmpty : public CVertexShaderConstantLayout
{
public:
    virtual CCharString PeekName();
};
extern CVertexShaderConstantLayoutEmpty Fable2DShaderConstantLayout_013ca8a8;
