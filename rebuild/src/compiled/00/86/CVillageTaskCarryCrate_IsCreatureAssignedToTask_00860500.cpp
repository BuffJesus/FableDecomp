#include "rebuild_abi.h"
// CVillageTaskCarryCrate::IsCreatureAssignedToTask @ 0x00860500
// add ecx,0x8; call helper; sub eax,[esp+4]; neg;sbb;inc; ret 4
// Return whether the current assignee (helper over the +0x8 subobject) equals arg.
struct CVillageTaskCarryCrateSub;
extern "C" long FABLE_FASTCALL FableAssignee_00860500(CVillageTaskCarryCrateSub *sub);
struct CVillageTaskCarryCrate { char pad00[0x8]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskCarryCrate::IsCreatureAssignedToTask(long who)
{
    return FableAssignee_00860500((CVillageTaskCarryCrateSub*)&m_sub[0]) == who;
}