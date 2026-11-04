// CSBCCorridor::GetType (retail 0x00A21F90; CSBCCorridor vtable 0x0129CCB0 slot 2, COL
// .?AVCSBCCorridor@@; no direct callers). Name: FableWin.pdb 031b3f80
// ?GetType@CSBCCorridor@@UAE?AW4ESB_TYPE@CSB@@XZ; value: Ego_r.pdb ESBC_CORRIDOR = 3.
#include "fable_steering_behaviour.h"

CSB::ESB_TYPE CSBCCorridor::GetType()
{
    return ESBC_CORRIDOR;
}
