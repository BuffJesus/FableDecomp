#include <cstdio>
bool __fastcall CScriptThing_IsEqualTo(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CScriptThing_IsEqualTo(&object, 1, 2)) { std::printf("AUTO_TINY_004aa8a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004aa8a0_TEST FAIL\n");
    return 1;
}