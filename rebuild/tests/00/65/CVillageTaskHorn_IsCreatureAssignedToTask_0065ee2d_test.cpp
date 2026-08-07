#include "rebuild_abi.h"
#include <cstdio>
struct CVillageTaskHornSub { long v; };
extern "C" long FABLE_FASTCALL FableAssignee_0065ee2d(CVillageTaskHornSub *sub){ return sub->v; }
struct CVillageTaskHorn { char pad00[0x10]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskHorn::IsCreatureAssignedToTask(long who){ return FableAssignee_0065ee2d((CVillageTaskHornSub*)&m_sub[0])==who; }
int main(){
    CVillageTaskHorn o; *(long*)&o.m_sub[0]=42;
    if(!o.IsCreatureAssignedToTask(42)){std::printf("B1\n");return 1;}
    if(o.IsCreatureAssignedToTask(7)){std::printf("B2\n");return 1;}
    std::printf("EQ_65ee2d_OK\n"); return 0;
}