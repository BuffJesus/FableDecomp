#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01229790()
{
    g_AutoTinyThunkObject.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01229790();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01229790_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229790_TEST FAIL\n");
    return 1;
}