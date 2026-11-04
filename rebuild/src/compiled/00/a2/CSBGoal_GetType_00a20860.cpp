// CSBGoal::GetType (retail 0x00A20860; CSBGoal vtable 0x0129CC34 slot 2, COL .?AVCSBGoal@@).
// Name: FableWin.pdb 031b3cc0 ?GetType@CSBGoal@@UAE?AW4ESB_TYPE@CSB@@XZ (rtti_port_compare
// 'agree'); value: Ego_r.pdb enum CSB::ESB_TYPE ESB_GOAL = 1.
#include "fable_steering_behaviour.h"

CSB::ESB_TYPE CSBGoal::GetType()
{
    return ESB_GOAL;
}
