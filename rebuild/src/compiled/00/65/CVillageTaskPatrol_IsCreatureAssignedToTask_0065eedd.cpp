#include "rebuild_abi.h"
// CVillageTaskPatrol::IsCreatureAssignedToTask @ 0x0065eedd
// add ecx,0x10; call helper; sub eax,[esp+4]; neg;sbb;inc; ret 4
// Return whether the current assignee (helper over the +0x10 subobject) equals arg.
struct CVillageTaskPatrolSub;
extern "C" long FABLE_FASTCALL FableAssignee_0065eedd(CVillageTaskPatrolSub *sub);
struct CVillageTaskPatrol { char pad00[0x10]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskPatrol::IsCreatureAssignedToTask(long who)
{
    return FableAssignee_0065eedd((CVillageTaskPatrolSub*)&m_sub[0]) == who;
}