#pragma optimize("s",on)
#include "engine/CTCHeroStats.h"

bool CTCHeroStats::HasCurrentMarriage() const
{
    return HasMarriageInState(MARRIAGE_STATE_1) ||
           HasMarriageInState(MARRIAGE_STATE_2) ||
           HasMarriageInState(MARRIAGE_STATE_3) ||
           HasMarriageInState(MARRIAGE_STATE_4);
}