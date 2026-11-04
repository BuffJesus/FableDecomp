#include "engine/CEnginePrimitiveManager2DSprites.h"
#include "engine/CEngineInternalPrimitiveBase.h"
void CEnginePrimitiveManager2DSprites::Render2DPrimitive(CEngineInternalPrimitiveBase& primitive,CEngineSubPrimitiveChain& chain,ESubPrimitiveType type){if(Enable){if(primitive.Type==ENGINE_PRIMITIVE_2D_CLOCK_SPRITE)RenderClockPrim(primitive,chain,type);else RenderSpritePrim(primitive,chain,type);}}
