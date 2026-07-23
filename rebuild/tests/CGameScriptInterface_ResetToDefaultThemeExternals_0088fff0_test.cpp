#include <cstdio>
static int g_a=-9,g_b=-9,g_c=-9;
struct Inner { int tag; };
static Inner* g_self=0;
struct Mid { char pad[0x18]; Inner* p18; };
struct Outer { char pad[4]; Mid* p4; };
void __fastcall Inner_Do(Inner* self, int e, int a, int b, int c){ (void)e; g_self=self; g_a=a; g_b=b; g_c=c; }
void __fastcall CGameScriptInterface_ResetToDefaultThemeExternals(Outer* self, int edx, int arg)
{
    Inner* obj = self->p4->p18;
    Inner_Do(obj, edx, 0, 1, arg);
}
int main(){
    static Inner theinner; theinner.tag=77;
    static Mid themid; themid.p18 = &theinner;
    static Outer theouter; theouter.p4 = &themid;
    CGameScriptInterface_ResetToDefaultThemeExternals(&theouter, 0, 555);
    if(g_self==&theinner && g_a==0 && g_b==1 && g_c==555){ std::printf("CGameScriptInterface_0088fff0_TEST PASS\n"); return 0; }
    std::printf("FAIL self=%p a=%d b=%d c=%d\n", (void*)g_self, g_a, g_b, g_c); return 1;
}