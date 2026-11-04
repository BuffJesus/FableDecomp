// ?Normalise@CRightHandedSet@@QAEXXZ @ 0x00a0be10 -- out-of-line copy of the
// inline CRightHandedSet::Normalise (FableWin.pdb names it as a function),
// emitted in the CCamera translation unit just before CCamera::SetView
// (0x00a0be30), which inlines the same body. Both call
// C3DVector::AccurateNormalise (0x00A14440) on Up (+0) and Forward (+0xC).
#include "fable_camera.h"

void (CRightHandedSet::*g_CRightHandedSetNormalise)() = &CRightHandedSet::Normalise;
