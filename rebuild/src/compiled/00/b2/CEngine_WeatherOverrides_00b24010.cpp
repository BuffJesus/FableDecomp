#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineWeatherRenderer.h"
void CEngine::OverrideRain(float strength)
{
    NEngineInternal::GPWeatherRenderer->OverrideRain(strength);
}
void CEngine::OverrideSnow(float strength)
{
    NEngineInternal::GPWeatherRenderer->OverrideSnow(strength);
}
