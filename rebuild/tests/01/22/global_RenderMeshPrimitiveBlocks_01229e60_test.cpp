#include <cstdio>
static int g_AutoTinyMethodOptionalCalls = 0;
static int g_AutoTinyMethodOptionalValue = 0;
static void* g_AutoTinyMethodOptionalObserved = 0;
struct AutoTinyMethodOptionalTarget
{
    void Invoke();
};
AutoTinyMethodOptionalTarget g_AutoTinyMethodOptionalObject;
void* g_AutoTinyMethodOptionalPointer = &g_AutoTinyMethodOptionalValue;
void AutoTinyMethodOptionalTarget::Invoke()
{
    ++g_AutoTinyMethodOptionalCalls;
}
void __cdecl AutoTinyMethodOptionalCall(void* object)
{
    g_AutoTinyMethodOptionalObserved = object;
}
void __fastcall RenderMeshPrimitiveBlocks()
{
    g_AutoTinyMethodOptionalObject.Invoke();
    if (g_AutoTinyMethodOptionalPointer != 0)
        AutoTinyMethodOptionalCall(g_AutoTinyMethodOptionalPointer);
}
int main()
{
    RenderMeshPrimitiveBlocks();
    if (g_AutoTinyMethodOptionalCalls == 1 && g_AutoTinyMethodOptionalObserved == g_AutoTinyMethodOptionalPointer) { std::printf("AUTO_TINY_01229e60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229e60_TEST FAIL\n");
    return 1;
}