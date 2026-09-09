#include "engine/CEngineWeatherRenderer.h"

EWeatherBoxVisible CEngineWeatherRenderer::ClipLockedBoxToCamera(
    const C3DVector& position, float extent, long boxIndex)
{
    if (!Settings.ParticleBoxLocked)
        SavedBoxVisibility[boxIndex] = ClipBoxToCamera(position, extent);
    return SavedBoxVisibility[boxIndex];
}