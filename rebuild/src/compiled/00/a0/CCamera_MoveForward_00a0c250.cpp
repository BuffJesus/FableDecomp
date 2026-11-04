#include "fable_camera.h"
void CCamera::MoveForward(float world_units)
{
    if (HeightLocked) {
        WorldPosition.X += RHSet.GetForward().X * world_units;
        WorldPosition.Y += RHSet.GetForward().Y * world_units;
    } else {
        WorldPosition += RHSet.GetForward() * world_units;
    }
}
