#include <stdio.h>
struct CTCMapwho { int x; };
static int g_clean=0, g_rel=0;
extern "C" void __fastcall Mapwho_Cleanup(CTCMapwho* self){ g_clean++; }
extern "C" void __cdecl Mapwho_Release(CTCMapwho* self){ g_rel++; }

void __fastcall OnDie(CTCMapwho* self);

int main(){
    CTCMapwho o;
    OnDie(&o);
    OnDie(0);
    if(g_clean==1 && g_rel==1) printf("OK_007f3f00\n");
    else printf("FAIL %d %d\n", g_clean, g_rel);
    return 0;
}