#include <cstdio>
static int g_ord_009c2520[2]; static int g_n_009c2520=0;
struct FableMember_009c2520; struct FableBase_009c2520;
extern "C" void __fastcall FableMemberDtor_009c2520(FableMember_009c2520* p){ g_ord_009c2520[g_n_009c2520++]=1; }
extern "C" void __fastcall FableBaseDtor_009c2520(FableBase_009c2520* p){ g_ord_009c2520[g_n_009c2520++]=2; }
extern "C" void __fastcall FableDualDestval_009c2520(char *self){ FableMemberDtor_009c2520((FableMember_009c2520*)(self+0x18)); FableBaseDtor_009c2520((FableBase_009c2520*)(self+0x14)); }
int main(){
    char buf[0x100]; FableDualDestval_009c2520(buf);
    if(g_n_009c2520==2 && g_ord_009c2520[0]==1 && g_ord_009c2520[1]==2){std::printf("DUALDV9C25_OK\n");return 0;}
    std::printf("FAIL\n"); return 1;
}