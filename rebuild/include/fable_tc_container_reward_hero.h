#pragma once
// Hand-owned (retail model): CTCContainerRewardHero, the thing component of a reward container that pops its items
// out to the hero (Ego_r.pdb: CTCContainerRewardHero : CTCContainer, 0x20 there; Enabled +0x1C,
// CreateObjectsThatTimeOut +0x1D). Partial view: only the members reconstructed so far are declared; the base and the
// data members are not.
#include "fable_2d_vector.h"

class CTCContainerRewardHero
{
protected:
    // Ego_r ?AdjustTrajectoryVector@CTCContainerRewardHero@@IBEXJAAVC3DVector@@@Z (retail 0x0074E080): the pop-out
    // direction as a unit vector scaled to 0.3, lifted by 0.2 per item (counting from one).
    void AdjustTrajectoryVector(long itemIndex, C3DVector& vector) const;
};
