#include "fable_camera.h"
CCamera::CCamera(const CCamera& other)
{
    WorldPosition.X=other.WorldPosition.X;
    WorldPosition.Y=other.WorldPosition.Y;
    WorldPosition.Z=other.WorldPosition.Z;
    RHSet.Up.X=other.RHSet.Up.X;
    RHSet.Up.Y=other.RHSet.Up.Y;
    RHSet.Up.Z=other.RHSet.Up.Z;
    RHSet.Forward.X=other.RHSet.Forward.X;
    RHSet.Forward.Y=other.RHSet.Forward.Y;
    RHSet.Forward.Z=other.RHSet.Forward.Z;
    HeightLocked=other.HeightLocked;
    ViewVecZLocked=other.ViewVecZLocked;
    FOVFlags=other.FOVFlags;
    HorizontalFOV=other.HorizontalFOV;
    VerticalFOV=other.VerticalFOV;
    Zoom=other.Zoom;
}
