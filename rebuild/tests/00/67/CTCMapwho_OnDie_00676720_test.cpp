#include <stdio.h>

struct CTCMapwho { int dummy; };

static int g_pre=0, g_free=0;
static CTCMapwho* g_freed=0;

extern "C" void __fastcall Mapwho_PreDie(CTCMapwho* self){ g_pre++; }
extern "C" void __cdecl retail_free(CTCMapwho* p){ g_free++; g_freed=p; }

void __fastcall OnDie(CTCMapwho* self);

int main(){
    CTCMapwho o; o.dummy=7;
    OnDie(&o);
    if(g_pre!=1||g_free!=1||g_freed!=&o) return 1;
    OnDie(0);
    if(g_pre!=1||g_free!=1) return 1;
    puts("OK_00676720");
    return 0;
}