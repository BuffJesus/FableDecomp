#include "engine/C2DBoxI.h"

void C2DBoxI::ExtendBordersBy(long)
{
    --TLX;
    ++BRX;
    --TLY;
    ++BRY;
}
