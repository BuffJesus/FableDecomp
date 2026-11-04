#include "engine/CEnginePrimitiveManagerSprites.h"
ERendered CEnginePrimitiveManagerSprites::RenderPrimitive(CEngineInternalPrimitiveBase& primitive,EEngineRenderLayer,CEngineSubPrimitiveChain&){if(!Enable)return RENDERED_NOTHING;RenderSprite(reinterpret_cast<CEngineInternalPrimitiveSprite&>(primitive));return RENDERED_OBJECT;}
