// CTCContainerRewardHero::AdjustTrajectoryVector (retail 0x0074E080, Ego_r lib_tc_container_reward_hero): normalise the
// pop-out vector, scale it to 0.3 and lift it by 0.2 for each item up to and including this one.
#include "fable_tc_container_reward_hero.h"

void CTCContainerRewardHero::AdjustTrajectoryVector(long itemIndex, C3DVector& vector) const
{
    vector.Normalise();
    vector *= 0.3f;
    vector.Z += (itemIndex + 1.0f) * 0.2f;
}
