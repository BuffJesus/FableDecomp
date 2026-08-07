#include "rebuild_abi.h"
#include <cstdio>
struct CVillageTaskTorchSub { long v; };
extern "C" long FABLE_FASTCALL FableAssignee_0065ed7a(CVillageTaskTorchSub *sub){ return sub->v; }
struct CVillageTaskTorch { char pad00[0x10]; char m_sub[4]; bool IsCreatureAssignedToTask(long who); };
bool CVillageTaskTorch::IsCreatureAssignedToTask(long who){ return FableAssignee_0065ed7a((CVillageTaskTorchSub*)&m_sub[0])==who; }
int main(){
    CVillageTaskTorch o; *(long*)&o.m_sub[0]=42;
    if(!o.IsCreatureAssignedToTask(42)){std::printf("B1\n");return 1;}
    if(o.IsCreatureAssignedToTask(7)){std::printf("B2\n");return 1;}
    std::printf("EQ_65ed7a_OK\n"); return 0;
}