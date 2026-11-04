#include "fable_engine.h"
#include "fable_engine_def.h"
#include "fable_engine_internal.h"
#include "engine/CEnginePrimitiveRenderer.h"
float CEngine::GetPrimitiveDrawDistanceFactor() const
{
    return (NEngineInternal::GPEngineDef->GamePrimitiveDefaultFadeRangeRatio+1.0f)
        * NEngineInternal::GPPrimitiveRenderer->DrawDistanceFactor;
}
