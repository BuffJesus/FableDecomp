#include "fable_camera.h"
void CCamera::MoveSideways(float world_units)
{
    if (HeightLocked) {
        WorldPosition.X += RHSet.GetRight().X * world_units;
        WorldPosition.Y += RHSet.GetRight().Y * world_units;
    } else {
        WorldPosition += RHSet.GetRight() * world_units;
    }
}
