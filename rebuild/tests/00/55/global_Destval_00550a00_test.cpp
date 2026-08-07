#include <cstdio>
static int g_ord_00550a00[2]; static int g_n_00550a00=0;
struct FableMember_00550a00; struct FableBase_00550a00;
extern "C" void __fastcall FableMemberDtor_00550a00(FableMember_00550a00* p){ g_ord_00550a00[g_n_00550a00++]=1; }
extern "C" void __fastcall FableBaseDtor_00550a00(FableBase_00550a00* p){ g_ord_00550a00[g_n_00550a00++]=2; }
extern "C" void __fastcall FableDualDestval_00550a00(char *self){ FableMemberDtor_00550a00((FableMember_00550a00*)(self+0x58)); FableBaseDtor_00550a00((FableBase_00550a00*)(self+0x24)); }
int main(){
    char buf[0x100]; FableDualDestval_00550a00(buf);
    if(g_n_00550a00==2 && g_ord_00550a00[0]==1 && g_ord_00550a00[1]==2){std::printf("DUALDV550A_OK\n");return 0;}
    std::printf("FAIL\n"); return 1;
}