#pragma once
// Canonical Ego_r.pdb hierarchy and retail offsets. Slot13=clock,slot14=sprite.
#include "fable_engine_primitive_manager.h"
#include "fable_pixel_shader.h"
#pragma pack(push,1)
class CEnginePrimitiveManager2DSprites : public CEnginePrimitiveManager {
public:
 virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType);
 virtual void GetSupportedTypes(CEnginePrimitiveTypeList&);
 virtual void Render2DPrimitive(CEngineInternalPrimitiveBase&,CEngineSubPrimitiveChain&,ESubPrimitiveType);
 virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);
 virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);
protected:
 virtual void RenderClockPrim(CEngineInternalPrimitiveBase&,CEngineSubPrimitiveChain&,ESubPrimitiveType);
 virtual void RenderSpritePrim(CEngineInternalPrimitiveBase&,CEngineSubPrimitiveChain&,ESubPrimitiveType);
public:
 CVertexShader VertexShader2DSprite,VertexShader2DClockSprite;
 CPixelShader PixelShader2DClockSprite,PixelShader2DClockSpriteAdditive;
 unsigned short Indices[6];
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManager2DSprites)==0x38);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManager2DSprites,Enable)==8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManager2DSprites,VertexShader2DSprite)==0xc);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManager2DSprites,VertexShader2DClockSprite)==0x14);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManager2DSprites,PixelShader2DClockSprite)==0x1c);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManager2DSprites,PixelShader2DClockSpriteAdditive)==0x24);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManager2DSprites,Indices)==0x2c);
