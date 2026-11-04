#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineShadowRenderer.h"
bool CEngine::DeallocateShadowBuffer()
{
    return NEngineInternal::GPShadowRenderer->XBoxDeallocateShadowBuffer();
}
void CEngine::ReallocateShadowBuffer()
{
    NEngineInternal::GPShadowRenderer->XBoxReallocateShadowBuffer();
}
