#include <cstdio>
int __fastcall CDoorDef_GetSizeofClass()
{
    return 60;
}
int main()
{
    if (CDoorDef_GetSizeofClass() == 60) { std::printf("AUTO_TINY_004d331c_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d331c_TEST FAIL\n");
    return 1;
}