#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineScreenEffectColourFilter.h"
#include "engine/CEngineVideoOptionsDef.h"

void CEngineWeatherRenderer::SetEffectQuality(const CEngineVideoOptionsDef& quality)
{
    RenderEnabled=quality.EnableWeatherEffects;
    MaximumWeatherParticlePercentage=quality.WeatherDensity;
}

void CEngineScreenEffectColourFilter::SetEffectQuality(const CEngineVideoOptionsDef& quality)
{
    if(quality.EnableColourFilter && EffectSupported)enabled=1;
    else enabled=0;
}
