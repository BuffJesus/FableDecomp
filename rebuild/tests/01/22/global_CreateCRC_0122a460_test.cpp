#include <cstdio>
static int g_AutoTinyReverseLoopCalls = 0;
struct AutoTinyReverseLoopTarget
{
    unsigned char storage[40];
    void Invoke();
};
AutoTinyReverseLoopTarget g_AutoTinyReverseLoopObjects[79] = {0};
void AutoTinyReverseLoopTarget::Invoke()
{
    ++g_AutoTinyReverseLoopCalls;
}
void __fastcall CreateCRC()
{
    AutoTinyReverseLoopTarget* cursor =
        g_AutoTinyReverseLoopObjects + 79;
    for (int remaining = 79; remaining != 0; --remaining)
    {
        --cursor;
        cursor->Invoke();
    }
}
int main()
{
    CreateCRC();
    if (g_AutoTinyReverseLoopCalls == 79) { std::printf("AUTO_TINY_0122a460_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a460_TEST FAIL\n");
    return 1;
}