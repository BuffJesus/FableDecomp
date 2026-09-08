
#include "engine/C2DBoxI.h"

int C2DBoxI::ContainsPoint(const C2DCoordI* p) const
{
    int x = p->X;
    if (x >= TLX && x < BRX)
    {
        int y = p->Y;
        if (y >= TLY && y < BRY)
            return 1;
    }
    return 0;
}
