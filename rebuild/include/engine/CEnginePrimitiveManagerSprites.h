#pragma once
#include "fable_engine_primitive_manager.h"
class CEngineInternalPrimitiveSprite;
class CEnginePrimitiveManagerSprites : public CEnginePrimitiveManager {
public:
 virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType);
 virtual void GetSupportedTypes(CEnginePrimitiveTypeList&);
 virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);
 virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);
protected:
 void RenderSprite(CEngineInternalPrimitiveSprite&);
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerSprites)==12);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerSprites,Enable)==8);
