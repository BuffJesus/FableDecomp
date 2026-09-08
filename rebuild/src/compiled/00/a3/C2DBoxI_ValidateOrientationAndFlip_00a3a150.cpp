#include "engine/C2DBoxI.h"

inline void SwapLong(long& left, long& right)
{
    long value = left;
    left = right;
    right = value;
}
void C2DBoxI::ValidateOrientationAndFlip()
{
    if (BRX < TLX) SwapLong(TLX, BRX);
    if (BRY < TLY) SwapLong(TLY, BRY);
}
