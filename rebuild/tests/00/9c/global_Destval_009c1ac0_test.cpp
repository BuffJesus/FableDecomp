#include <cstdio>
static int g_ord_009c1ac0[2]; static int g_n_009c1ac0=0;
struct FableMember_009c1ac0; struct FableBase_009c1ac0;
extern "C" void __fastcall FableMemberDtor_009c1ac0(FableMember_009c1ac0* p){ g_ord_009c1ac0[g_n_009c1ac0++]=1; }
extern "C" void __fastcall FableBaseDtor_009c1ac0(FableBase_009c1ac0* p){ g_ord_009c1ac0[g_n_009c1ac0++]=2; }
extern "C" void __fastcall FableDualDestval_009c1ac0(char *self){ FableMemberDtor_009c1ac0((FableMember_009c1ac0*)(self+0x18)); FableBaseDtor_009c1ac0((FableBase_009c1ac0*)(self+0x14)); }
int main(){
    char buf[0x100]; FableDualDestval_009c1ac0(buf);
    if(g_n_009c1ac0==2 && g_ord_009c1ac0[0]==1 && g_ord_009c1ac0[1]==2){std::printf("DUALDV9C1A_OK\n");return 0;}
    std::printf("FAIL\n"); return 1;
}