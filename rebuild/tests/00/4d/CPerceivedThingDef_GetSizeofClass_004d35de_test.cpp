#include <cstdio>
int __fastcall CPerceivedThingDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (CPerceivedThingDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_004d35de_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d35de_TEST FAIL\n");
    return 1;
}