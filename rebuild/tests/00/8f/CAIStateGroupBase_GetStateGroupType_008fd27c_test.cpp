#include <cstdio>
int __fastcall CAIStateGroupBase_GetStateGroupType()
{
    return 3;
}
int main()
{
    if (CAIStateGroupBase_GetStateGroupType() == 3) { std::printf("AUTO_TINY_008fd27c_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fd27c_TEST FAIL\n");
    return 1;
}