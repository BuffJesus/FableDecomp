#include "engine/CEngineWeatherRenderer.h"

void CEngineWeatherRenderer::AddWeatherMask(const CWeatherMask& mask, unsigned long)
{
    WeatherMaskList[CurrentMask] = mask;
    CurrentMask = (CurrentMask + 1) % 3;
}