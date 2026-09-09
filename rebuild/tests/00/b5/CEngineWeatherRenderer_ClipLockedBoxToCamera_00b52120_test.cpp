#include <stdio.h>
#include <string.h>
#include "engine/CEngineWeatherRenderer.h"

static EWeatherBoxVisible g_nextVisibility;
static int g_clipCalls;

EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToCamera(
    const C3DVector&, float) const
{
    ++g_clipCalls;
    return g_nextVisibility;
}

int main()
{
    CEngineWeatherRenderer renderer;
    C3DVector position = { 1.0f, 2.0f, 3.0f };

    memset(&renderer, 0, sizeof(renderer));
    g_nextVisibility = 7;
    if (renderer.ClipLockedBoxToCamera(position, 4.0f, 12) != 7) return 1;
    if (renderer.SavedBoxVisibility[12] != 7 || g_clipCalls != 1) return 2;

    renderer.Settings.ParticleBoxLocked = true;
    g_nextVisibility = 9;
    if (renderer.ClipLockedBoxToCamera(position, 4.0f, 12) != 7) return 3;
    if (g_clipCalls != 1) return 4;

    printf("LOCKED_WEATHER_BOX PASS\n");
    return 0;
}