#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DSPRITE_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DSPRITE_H
// Canonical Ego_r.pdb inheritance; retail mask virtualslot28.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineInternalPrimitive2DSprite = 0x8c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "engine/CEngineInternalPrimitiveBase.h"
#include "fable_engine_primitive_2d_sprite.h"

class CTexture;

#pragma pack(push, 1)
class CEngineInternalPrimitive2DSprite : public CEngineInternalPrimitiveBase {
public:
    virtual void Update(const CEnginePrimitive&,unsigned long);
    virtual unsigned long GetRenderLayerMask();
    C2DBoxF       ClipBox;                                   // +0x48
    EEngine2DLayer Layer;                                    // +0x58
    unsigned long BankIndex;                                 // +0x5c
    unsigned long Frame;                                     // +0x60
    float         StartAngle;                                // +0x64
    float         EndAngle;                                  // +0x68
    CRGBColour    Colour;                                    // +0x6c
    CTexture*     Texture;                                   // +0x70
    bool          Filter;                                    // +0x74
    C2DVector     UVOffsetsStart;                            // +0x75 (packed)
    C2DVector     UVOffsetsEnd;                              // +0x7d (packed)
    bool          WrapTexture;                               // +0x85
    unsigned char _pad_0x86[0x2];                            // +0x86
    EEnginePrimitiveBlendMode BlendMode;                     // +0x88
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitive2DSprite) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, ClipBox) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, Layer) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, BankIndex) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, Frame) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, StartAngle) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, EndAngle) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, Colour) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, Texture) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, Filter) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, UVOffsetsStart) == 0x75);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, UVOffsetsEnd) == 0x7d);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, WrapTexture) == 0x85);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitive2DSprite, BlendMode) == 0x88);

#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DSPRITE_H
