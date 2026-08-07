#include <cstdio>
static int g_ord_006568bb[2]; static int g_n_006568bb=0;
struct FableMember_006568bb; struct FableBase_006568bb;
extern "C" void __fastcall FableMemberDtor_006568bb(FableMember_006568bb* p){ g_ord_006568bb[g_n_006568bb++]=1; }
extern "C" void __fastcall FableBaseDtor_006568bb(FableBase_006568bb* p){ g_ord_006568bb[g_n_006568bb++]=2; }
extern "C" void __fastcall FableDualDestval_006568bb(char *self){ FableMemberDtor_006568bb((FableMember_006568bb*)(self+0x20)); FableBaseDtor_006568bb((FableBase_006568bb*)(self+0x10)); }
int main(){
    char buf[0x100]; FableDualDestval_006568bb(buf);
    if(g_n_006568bb==2 && g_ord_006568bb[0]==1 && g_ord_006568bb[1]==2){std::printf("DUALDV6568_OK\n");return 0;}
    std::printf("FAIL\n"); return 1;
}