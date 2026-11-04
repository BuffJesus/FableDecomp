#include "fable_camera.h"
void CCamera::SetView(const C3DVector& view_direction,const C3DVector& up)
{
    RHSet.Init(up,view_direction);
    RHSet.Normalise();
}
