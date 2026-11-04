#pragma once

// CKinematic: Ego_r.pdb class CKinematic (sizeof 0x8): one private C2DVector Vector at +0x0.
// Used by value as CSB::Output (+0x04) and CSBInput::Input (+0x0C) (fable_steering_behaviour.h);
// FableWin.pdb ?SetInput@CSBInput@@QAEXVCKinematic@@@Z passes it by value.
// Ego_r/FableWin also list ??0CKinematic@@QAE@XZ, ??0CKinematic@@QAE@ABVC2DVector@@@Z,
// ?GetVector@CKinematic@@QBEABVC2DVector@@XZ, ?SetVector@CKinematic@@QAEXABVC2DVector@@@Z and a
// user-declared ??4CKinematic@@QAEABV0@ABV0@@Z (returns const CKinematic&). None has a recovered
// retail body or a confirmed retail address, so only the data member is modelled (as CWall).
// Ego_r packs C2DVector (DIA packed flag, alignment 1); fable_2d_vector.h does not, so CKinematic
// is 4-aligned here. Every current use sits at a 4-aligned offset, so the layout is unchanged.

#include "rebuild_abi.h"
#include "fable_2d_vector.h"

class CKinematic
{
private:
    C2DVector Vector;   // +0x0
};

FABLE_STATIC_ASSERT(sizeof(CKinematic) == 0x8);
