#include "rebuild_abi.h"
// CVillageTaskSecuritySweep::IsCreatureAssignedToTask @ 0x00860fa0
// add ecx,0x8; call helper; sub eax,[esp+4]; neg;sbb;inc; ret 4
// Return whether the current assignee (helper over the +0x8 subobject) equals arg.
struct CVillageTaskSecuritySweepSub;
extern "C" long FABLE_FASTCALL FableAssignee_00860fa0(CVillageTaskSecuritySweepSub *sub);
struct CVillageTaskSecuritySweep { char pad00[0x8]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskSecuritySweep::IsCreatureAssignedToTask(long who)
{
    return FableAssignee_00860fa0((CVillageTaskSecuritySweepSub*)&m_sub[0]) == who;
}