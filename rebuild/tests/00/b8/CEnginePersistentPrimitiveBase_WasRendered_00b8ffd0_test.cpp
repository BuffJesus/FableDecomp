#include <cstdio>
bool __fastcall CEnginePersistentPrimitiveBase_WasRendered()
{
    return true;
}
int main()
{
    if (CEnginePersistentPrimitiveBase_WasRendered() == true) { std::printf("AUTO_TINY_00b8ffd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b8ffd0_TEST FAIL\n");
    return 1;
}