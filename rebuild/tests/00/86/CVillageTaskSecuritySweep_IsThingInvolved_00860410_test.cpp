#include "rebuild_abi.h"
#include <cstdio>
struct CVillageTaskSecuritySweepSub { long v; };
extern "C" long FABLE_FASTCALL FableAssignee_00860410(CVillageTaskSecuritySweepSub *sub){ return sub->v; }
struct CVillageTaskSecuritySweep { char pad00[0x8]; char m_sub[4]; bool IsThingInvolved(long who); };
bool CVillageTaskSecuritySweep::IsThingInvolved(long who){ return FableAssignee_00860410((CVillageTaskSecuritySweepSub*)&m_sub[0])==who; }
int main(){
    CVillageTaskSecuritySweep o; *(long*)&o.m_sub[0]=42;
    if(!o.IsThingInvolved(42)){std::printf("B1\n");return 1;}
    if(o.IsThingInvolved(7)){std::printf("B2\n");return 1;}
    std::printf("EQ_860410_OK\n"); return 0;
}