#include "rebuild_abi.h"
// CVillageTaskPickCrops::IsCreatureAssignedToTask @ 0x0065ef72
// add ecx,0x10; call helper; sub eax,[esp+4]; neg;sbb;inc; ret 4
// Return whether the current assignee (helper over the +0x10 subobject) equals arg.
struct CVillageTaskPickCropsSub;
extern "C" long FABLE_FASTCALL FableAssignee_0065ef72(CVillageTaskPickCropsSub *sub);
struct CVillageTaskPickCrops { char pad00[0x10]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskPickCrops::IsCreatureAssignedToTask(long who)
{
    return FableAssignee_0065ef72((CVillageTaskPickCropsSub*)&m_sub[0]) == who;
}