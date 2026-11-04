#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineScreenEffectColourFilter.h"
#include "engine/CEngineWaterRenderer.h"

void CEngine::SetSpotlightShadowMode(bool on)
{
    NEngineInternal::GPLightingManager->ShadowMode=on
        ? LIGHTING_MANAGER_SHADOW_MODE_SPOT_LIGHTS
        : LIGHTING_MANAGER_SHADOW_MODE_MAIN_LIGHT;
}
void CEngine::SetScreenColourFilter(const CEngineColourFilterSettings& settings)
{
    NEngineInternal::GPScreenEffectColourFilter->SetFilterSettings(settings);
}
void CEngine::SetSeaBodyToRender(CCharString name)
{
    NEngineInternal::GPWaterRenderer->SetSeaBodyToRender(name);
}
