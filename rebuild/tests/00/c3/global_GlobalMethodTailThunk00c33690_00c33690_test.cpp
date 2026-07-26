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
void __fastcall GlobalMethodTailThunk_00c33690()
{
    g_AutoTinyThunkObject.Invoke();
}
int main()
{
    GlobalMethodTailThunk_00c33690();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_00c33690_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00c33690_TEST FAIL\n");
    return 1;
}