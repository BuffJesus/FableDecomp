#include "candidates/mouse_dx_accessors_compiled.h"

const C2DVector& CMouseDX::PeekPos() const
{
    return *reinterpret_cast<const C2DVector*>(&PositionX);
}
