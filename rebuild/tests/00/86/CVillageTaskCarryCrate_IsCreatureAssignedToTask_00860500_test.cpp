#include "rebuild_abi.h"
#include <cstdio>
struct CVillageTaskCarryCrateSub { long v; };
extern "C" long FABLE_FASTCALL FableAssignee_00860500(CVillageTaskCarryCrateSub *sub){ return sub->v; }
struct CVillageTaskCarryCrate { char pad00[0x8]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskCarryCrate::IsCreatureAssignedToTask(long who){ return FableAssignee_00860500((CVillageTaskCarryCrateSub*)&m_sub[0])==who; }
int main(){
    CVillageTaskCarryCrate o; *(long*)&o.m_sub[0]=42;
    if(!o.IsCreatureAssignedToTask(42)){std::printf("B1\n");return 1;}
    if(o.IsCreatureAssignedToTask(7)){std::printf("B2\n");return 1;}
    std::printf("EQ_860500_OK\n"); return 0;
}