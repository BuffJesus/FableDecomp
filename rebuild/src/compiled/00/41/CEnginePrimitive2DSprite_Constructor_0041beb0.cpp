#include "fable_engine_primitive_2d_sprite.h"
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 theme,const C2DBoxF& box,
    EEngine2DLayer layer,unsigned long bank,unsigned long frame,
    EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,
    float start,float end,unsigned char alpha,unsigned char red,
    unsigned char green,unsigned char blue,unsigned long flags,
    const C2DVector& uvStart,const C2DVector& uvEnd)
    :CEnginePrimitive(ENGINE_PRIMITIVE_2D_SPRITE,theme,interpolation),ClipBox(box),
    Layer(layer),BankIndex(bank),Frame(frame),StartAngle(start),EndAngle(end),
    BlendMode(blend),Colour(red,green,blue,alpha),ClipBoxIsPos(false),Flags(flags),
    Texture(0),UVOffsetsStart(uvStart),UVOffsetsEnd(uvEnd)
{}
