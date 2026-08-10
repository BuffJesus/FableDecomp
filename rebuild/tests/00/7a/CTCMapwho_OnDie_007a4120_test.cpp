#include <stdio.h>

struct CTCMapwho { int tag; };

static int g_member = 0;
static int g_helper = 0;
static CTCMapwho* g_last = 0;

extern "C" void __fastcall Mapwho_Member(CTCMapwho* self){ g_member++; g_last = self; }
extern "C" int  __cdecl    Mapwho_Helper(CTCMapwho* self){ g_helper++; g_last = self; return 0; }

void __fastcall OnDie(CTCMapwho* self);

int main(){
    OnDie(0);
    if (g_member != 0 || g_helper != 0) { printf("FAIL null\n"); return 1; }
    CTCMapwho obj; obj.tag = 7;
    OnDie(&obj);
    if (g_member != 1 || g_helper != 1) { printf("FAIL count\n"); return 1; }
    if (g_last != &obj) { printf("FAIL arg\n"); return 1; }
    printf("OK_007a4120\n");
    return 0;
}