// C3DCoordI::operator+=(const C3DVector&) (Ego_r ??YC3DCoordI@@QAEABV0@ABVC3DVector@@@Z),
// retail 0x00A13BE0: add each float component to the integer point, truncating
// through _ftol. Retail order: ??YC2DCoordI 0xA13B80, ??ZC2DCoordI 0xA13BB0, this,
// ??ZC3DCoordI 0xA13C20 (Ego_r keeps the same four in the same order).
#include "fable_display_geometry.h"
#include "fable_2d_vector.h"

const C3DCoordI& C3DCoordI::operator+=(const C3DVector& offset)
{
    X = (long)(X + offset.X);
    Y = (long)(Y + offset.Y);
    Z = (long)(Z + offset.Z);
    return *this;
}
