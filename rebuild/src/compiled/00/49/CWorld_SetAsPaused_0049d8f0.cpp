#include "fable_world.h"

void CWorld::SetAsPaused(bool paused)
{
    if (paused!=Paused)
        Paused=paused;
}
