#include <cstdio>
static int g_AutoTinyReverseLoopCalls = 0;
struct AutoTinyReverseLoopTarget
{
    unsigned char storage[4];
    void Invoke();
};
AutoTinyReverseLoopTarget g_AutoTinyReverseLoopObjects[4] = {0};
void AutoTinyReverseLoopTarget::Invoke()
{
    ++g_AutoTinyReverseLoopCalls;
}
void __fastcall CreateCRC()
{
    AutoTinyReverseLoopTarget* cursor =
        g_AutoTinyReverseLoopObjects + 4;
    for (int remaining = 4; remaining != 0; --remaining)
    {
        --cursor;
        cursor->Invoke();
    }
}
int main()
{
    CreateCRC();
    if (g_AutoTinyReverseLoopCalls == 4) { std::printf("AUTO_TINY_01229df0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229df0_TEST FAIL\n");
    return 1;
}