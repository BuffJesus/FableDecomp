#pragma once
#include <stddef.h>
#include "fable_engine_primitive.h"
#include "fable_display_geometry.h"
#include "fable_2d_vector.h"
#include "fable_rgb_colour.h"
enum EEngine2DLayer;
enum EEnginePrimitiveBlendMode;
class CTexture;
// Ego_r.pdb original inheritance and fields; retail 0041BEB0 confirms all
// offsets and preserves the base Pad and the three bool-alignment bytes.
class CEnginePrimitive2DSprite : public CEnginePrimitive
{
public:
    CEnginePrimitive2DSprite(unsigned __int64 theme,const C2DBoxF& box,
        EEngine2DLayer layer,CTexture& texture,
        EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,
        float start,float end,unsigned char alpha,unsigned char red,
        unsigned char green,unsigned char blue,unsigned long flags,
        const C2DVector& uvStart,const C2DVector& uvEnd);
    CEnginePrimitive2DSprite(unsigned __int64 theme,const C2DBoxF& box,
        EEngine2DLayer layer,unsigned long bank,unsigned long frame,
        EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,
        float start,float end,unsigned char alpha,unsigned char red,
        unsigned char green,unsigned char blue,unsigned long flags,
        const C2DVector& uvStart,const C2DVector& uvEnd);
    C2DBoxF ClipBox;
    EEngine2DLayer Layer;
    unsigned long BankIndex,Frame;
    float StartAngle,EndAngle;
    EEnginePrimitiveBlendMode BlendMode;
    CRGBColour Colour;
    bool ClipBoxIsPos;
    unsigned long Flags;
    CTexture* Texture;
    C2DVector UVOffsetsStart,UVOffsetsEnd;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DSprite)==84);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitive2DSprite,ClipBox)==12);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitive2DSprite,Colour)==52);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitive2DSprite,Texture)==64);
