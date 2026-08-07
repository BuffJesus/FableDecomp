#include "rebuild_abi.h"
#include <cstdio>
struct CTCInventoryBase { char pad[4]; int m_max; int m_cur; };
extern "C" int FABLE_FASTCALL FableGetSel_c8d4f0(CTCInventoryBase *self, int delta){
    int v=self->m_cur+delta; if(v>self->m_max) return -1; self->m_cur=v; return 0; }
int main(){ CTCInventoryBase o; o.m_max=10; o.m_cur=3;
 if(FableGetSel_c8d4f0(&o,2)!=0||o.m_cur!=5){std::printf("B1\n");return 1;}
 if(FableGetSel_c8d4f0(&o,100)!=-1){std::printf("B2\n");return 1;}
 std::printf("GS_c8d4f0_OK\n"); return 0; }