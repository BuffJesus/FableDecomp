#include <cstdio>
typedef void (__fastcall *GiveHeroTitleFn)(void*, int, int, int);
struct CGameScriptInterface { void* vt; };
static int g_title = 0; static int g_a = 0; static int g_b = 0;
void __fastcall target(void* self, int title, int a, int b){ g_title = title; g_a = a; g_b = b; }
void __fastcall CGameScriptInterface_GiveHeroTitle(CGameScriptInterface* self, void* /*edx*/, int title)
{
    GiveHeroTitleFn fn = *(GiveHeroTitleFn*)(*(char**)self + 0x1e4);
    fn(self, title, -1, 1);
}
int main(){
    static char blob[0x400];
    GiveHeroTitleFn tp = target;
    *(GiveHeroTitleFn*)(blob + 0x1e4) = tp;
    void* vt = blob;
    CGameScriptInterface obj; obj.vt = vt;
    CGameScriptInterface_GiveHeroTitle(&obj, 0, 42);
    if (g_title==42 && g_a==-1 && g_b==1) { std::printf("CGameScriptInterface_0088e110_TEST PASS\n"); return 0; }
    std::printf("FAIL title=%d a=%d b=%d\n", g_title, g_a, g_b); return 1;
}