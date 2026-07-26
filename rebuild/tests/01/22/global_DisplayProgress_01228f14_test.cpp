#include <cstdio>
static int g_AutoTinyOptionalValue = 0;
static void* g_AutoTinyObservedObject = 0;
void* g_AutoTinyOptionalObject = &g_AutoTinyOptionalValue;
void __cdecl AutoTinyOptionalTarget(void* object)
{
    g_AutoTinyObservedObject = object;
}
void __fastcall DisplayProgress()
{
    if (g_AutoTinyOptionalObject != 0)
        AutoTinyOptionalTarget(g_AutoTinyOptionalObject);
}
int main()
{
    DisplayProgress();
    if (g_AutoTinyObservedObject == g_AutoTinyOptionalObject) { std::printf("AUTO_TINY_01228f14_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228f14_TEST FAIL\n");
    return 1;
}