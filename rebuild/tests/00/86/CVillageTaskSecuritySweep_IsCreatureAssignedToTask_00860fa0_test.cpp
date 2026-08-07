#include "rebuild_abi.h"
#include <cstdio>
struct CVillageTaskSecuritySweepSub { long v; };
extern "C" long FABLE_FASTCALL FableAssignee_00860fa0(CVillageTaskSecuritySweepSub *sub){ return sub->v; }
struct CVillageTaskSecuritySweep { char pad00[0x8]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskSecuritySweep::IsCreatureAssignedToTask(long who){ return FableAssignee_00860fa0((CVillageTaskSecuritySweepSub*)&m_sub[0])==who; }
int main(){
    CVillageTaskSecuritySweep o; *(long*)&o.m_sub[0]=42;
    if(!o.IsCreatureAssignedToTask(42)){std::printf("B1\n");return 1;}
    if(o.IsCreatureAssignedToTask(7)){std::printf("B2\n");return 1;}
    std::printf("EQ_860fa0_OK\n"); return 0;
}