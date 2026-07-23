#include <cstdio>
struct CSummonerDef { int v; };
static int g_called = 0;
static const CSummonerDef* g_src = 0;
static CSummonerDef* g_self = 0;
extern "C" void __fastcall base_copy(CSummonerDef* self, const CSummonerDef* src)
{
    g_called = 1;
    g_self = self;
    g_src = src;
}
void __fastcall CSummonerDef_Copy(CSummonerDef* self, const CSummonerDef* src)
{
    base_copy(self, src);
}
int main()
{
    CSummonerDef a; a.v = 1;
    CSummonerDef b; b.v = 2;
    CSummonerDef_Copy(&a, &b);
    if (g_called == 1 && g_self == &a && g_src == &b) {
        std::printf("CSummonerDef_00454267_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}