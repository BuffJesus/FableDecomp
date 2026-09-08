#include "engine/C2DBoxI.h"

void C2DBoxI::ExtendToContain(const C2DCoordI& point)
{
    if (TLX > point.X) TLX = point.X;
    if (BRX < point.X) BRX = point.X;
    if (TLY > point.Y) TLY = point.Y;
    if (BRY < point.Y) BRY = point.Y;
}
