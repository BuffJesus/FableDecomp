// CNavigatorAStar::GetCurrentDestination (retail 0x00A224C0, vtable 0x0129CCCC slot 12;
// Ego_r ?GetCurrentDestination@CNavigatorAStar@@UBEABVC3DVector@@XZ 0xB8B2C0 returns
// PosAskedToGetTo). Retail STLport layout puts PosAskedToGetTo at +0x7C (fable_navigator.h).
#include "fable_navigator.h"

const C3DVector& CNavigatorAStar::GetCurrentDestination() const
{
    return PosAskedToGetTo;
}
