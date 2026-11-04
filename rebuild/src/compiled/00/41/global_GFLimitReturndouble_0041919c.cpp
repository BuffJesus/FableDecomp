#include "fable_game.h"

// VC7.1 emits the retail helper's single return path under size optimization.
#pragma optimize("s", on)
template<>
const double& FABLE_FASTCALL GFLimitReturn<double>(
    const double& value, const double& minimum, const double& maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}
