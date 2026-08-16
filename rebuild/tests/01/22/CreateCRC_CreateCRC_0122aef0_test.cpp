#include <cstdio>
static int g_AutoTinyReverseLoopCalls = 0;
struct AutoTinyReverseLoopTarget
{
    unsigned char storage[4];
    void Invoke();
};
AutoTinyReverseLoopTarget g_AutoTinyReverseLoopObjects[3] = {0};
void AutoTinyReverseLoopTarget::Invoke()
{
    ++g_AutoTinyReverseLoopCalls;
}
void __fastcall CreateCRC()
{
    AutoTinyReverseLoopTarget* cursor =
        g_AutoTinyReverseLoopObjects + 3;
    for (int remaining = 3; remaining != 0; --remaining)
    {
        --cursor;
        cursor->Invoke();
    }
}
int main()
{
    CreateCRC();
    if (g_AutoTinyReverseLoopCalls == 3) { std::printf("AUTO_TINY_0122aef0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122aef0_TEST FAIL\n");
    return 1;
}