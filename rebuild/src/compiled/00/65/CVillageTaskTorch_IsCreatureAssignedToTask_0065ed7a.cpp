#include "rebuild_abi.h"
// CVillageTaskTorch::IsCreatureAssignedToTask @ 0x0065ed7a
// add ecx,0x10; call helper; sub eax,[esp+4]; neg;sbb;inc; ret 4
// Return whether the current assignee (helper over the +0x10 subobject) equals arg.
struct CVillageTaskTorchSub;
extern "C" long FABLE_FASTCALL FableAssignee_0065ed7a(CVillageTaskTorchSub *sub);
struct CVillageTaskTorch { char pad00[0x10]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskTorch::IsCreatureAssignedToTask(long who)
{
    return FableAssignee_0065ed7a((CVillageTaskTorchSub*)&m_sub[0]) == who;
}