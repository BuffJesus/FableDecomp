#include "engine/CEngineWeatherRenderer.h"
void CEngineWeatherRenderer::OverrideRain(float strength)
{
    if(strength>=0.0f){
        float clamped = strength<MaximumWeatherParticlePercentage ? strength : MaximumWeatherParticlePercentage;
        RainOverriden=true;
        OverridenRainStrength=clamped;
    }else RainOverriden=false;
}
void CEngineWeatherRenderer::OverrideSnow(float strength)
{
    if(strength>=0.0f){
        float clamped = strength<MaximumWeatherParticlePercentage ? strength : MaximumWeatherParticlePercentage;
        SnowOverriden=true;
        OverridenSnowStrength=clamped;
    }else SnowOverriden=false;
}
