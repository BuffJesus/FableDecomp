#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
void CEngine::SetGlobalLightingParameters(const C3DVector& direction,const CRGBFloatColour& ambient,const CRGBFloatColour& diffuse,const CRGBFloatColour& backlight)
{
    NEngineInternal::GPLightingManager->SetGlobalLightingParameters(direction,ambient,diffuse,backlight);
}
void CEngine::Set2DPrimitiveLightingParameters(const C3DVector& direction,const CRGBFloatColour& ambient,const CRGBFloatColour& diffuse,const CRGBFloatColour& backlight)
{
    NEngineInternal::GPPrimitiveRenderer2D->SetLightingParameters(direction,ambient,diffuse,backlight);
}
