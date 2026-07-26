#include "candidates/mouse_dx_accessors_compiled.h"

void CMouseDX::GetPos(C2DVector* output) const
{
    output->X = PositionX;
    output->Y = PositionY;
}
