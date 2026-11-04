#include "fable_display_engine.h"
#include "fable_iengine.h"

void CDisplayEngine::PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth)
{
    Engine3D->PostChangeResolution(width, height, depth);
}
