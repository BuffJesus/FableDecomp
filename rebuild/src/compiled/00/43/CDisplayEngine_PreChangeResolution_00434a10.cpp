#include "fable_display_engine.h"
#include "fable_iengine.h"

void CDisplayEngine::PreChangeResolution()
{
    Engine3D->PreChangeResolution();
}
