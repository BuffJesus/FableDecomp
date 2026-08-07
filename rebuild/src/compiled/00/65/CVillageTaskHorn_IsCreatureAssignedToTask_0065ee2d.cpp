#include "rebuild_abi.h"
// CVillageTaskHorn::IsCreatureAssignedToTask @ 0x0065ee2d
// add ecx,0x10; call helper; sub eax,[esp+4]; neg;sbb;inc; ret 4
// Return whether the current assignee (helper over the +0x10 subobject) equals arg.
struct CVillageTaskHornSub;
extern "C" long FABLE_FASTCALL FableAssignee_0065ee2d(CVillageTaskHornSub *sub);
struct CVillageTaskHorn { char pad00[0x10]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskHorn::IsCreatureAssignedToTask(long who)
{
    return FableAssignee_0065ee2d((CVillageTaskHornSub*)&m_sub[0]) == who;
}