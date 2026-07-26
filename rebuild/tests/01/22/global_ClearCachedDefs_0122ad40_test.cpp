#include <cstdio>
static int g_AutoTinyFieldFirstCalls = 0;
static int g_AutoTinyFieldSecondCalls = 0;
struct AutoTinyFieldMethodsTarget
{
    unsigned int value;
    void First();
    void Second();
};
AutoTinyFieldMethodsTarget g_AutoTinyFieldMethodsObject = {0};
void AutoTinyFieldMethodsTarget::First()
{
    ++g_AutoTinyFieldFirstCalls;
}
void AutoTinyFieldMethodsTarget::Second()
{
    ++g_AutoTinyFieldSecondCalls;
}
void __fastcall ClearCachedDefs()
{
    g_AutoTinyFieldMethodsObject.value = 0x012a20fc;
    g_AutoTinyFieldMethodsObject.First();
    g_AutoTinyFieldMethodsObject.Second();
}
int main()
{
    ClearCachedDefs();
    if (g_AutoTinyFieldMethodsObject.value == 0x012a20fc && g_AutoTinyFieldFirstCalls == 1 && g_AutoTinyFieldSecondCalls == 1) { std::printf("AUTO_TINY_0122ad40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ad40_TEST FAIL\n");
    return 1;
}