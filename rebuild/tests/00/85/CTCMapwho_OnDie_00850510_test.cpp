#include <stdio.h>
struct CTCMapwho { int dummy; };
static int g_pre=0, g_free=0;
extern "C" void __fastcall Mapwho_PreDie(CTCMapwho* self){ g_pre++; }
extern "C" void __cdecl Mapwho_Free(void* p){ g_free++; }
void __fastcall OnDie_Mapwho(CTCMapwho* self);
int main(){
    CTCMapwho m; m.dummy=1;
    OnDie_Mapwho(&m);
    OnDie_Mapwho(0);
    if(g_pre==1 && g_free==1) printf("OK_00850510\n");
    else printf("FAIL %d %d\n", g_pre, g_free);
    return 0;
}