#include "engine/CEngineInternalPrimitive2DSprite.h"
#include "fable_engine_primitive_2d_sprite.h"
#include "fable_surface_types.h"
#include "fable_engine_internal.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
void CEngineInternalPrimitive2DSprite::Update(const CEnginePrimitive& value,unsigned long flags)
{
    const CEnginePrimitive2DSprite& sprite=static_cast<const CEnginePrimitive2DSprite&>(value);
    EnginePrimitiveFlags=flags;
    ClipBox=sprite.ClipBox;Layer=sprite.Layer;BankIndex=sprite.BankIndex;Frame=sprite.Frame;
    StartAngle=sprite.StartAngle;EndAngle=sprite.EndAngle;BlendMode=sprite.BlendMode;
    Colour=sprite.Colour;*(EEngineInterpolationSet*)InterpolationSet=sprite.InterpolationSet;
    Texture=sprite.Texture;Filter=(sprite.Flags&2)!=0;
    UVOffsetsStart=sprite.UVOffsetsStart;UVOffsetsEnd=sprite.UVOffsetsEnd;
    WrapTexture=UVOffsetsStart.IsNonZero()||UVOffsetsEnd.IsNonZero();
    if(flags&0x80)EnginePrimitiveFlags=flags|0x80;
    RenderLayerMask=(flags&0x400)?0x00200000:0x80000000;
    if(BankIndex){
        const CGraphicInfoFrame* frame=NEngineInternal::GPResourceManager->GraphicBanks->PeekGraphicInfoFrame(BankIndex,Frame);
        frame->BeginLoading(0,0);
        if(sprite.ClipBoxIsPos){
            if(sprite.Flags&1){
                unsigned long halfWidth=frame->GetWidth()>>1,halfHeight=frame->GetHeight()>>1;
                ClipBox.left-=halfWidth;ClipBox.top-=halfHeight;
                ClipBox.right+=halfWidth;ClipBox.bottom+=halfHeight;
            }else{ClipBox.right+=frame->GetWidth();ClipBox.bottom+=frame->GetHeight();}
        }
    }else if(sprite.Texture&&sprite.ClipBoxIsPos){
        ClipBox.right+=Texture->GetWidth();ClipBox.bottom+=Texture->GetHeight();
    }
}
