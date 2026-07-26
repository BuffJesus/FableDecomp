#include <cstdio>
int __fastcall CNavNavigableLeafNode_GetRegion(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 16);
}
int main()
{
    unsigned char object[20] = {0};
    *reinterpret_cast<int*>(object + 16) = 0x12345678;
    if (CNavNavigableLeafNode_GetRegion(object) == 0x12345678) { std::printf("AUTO_TINY_00a75a80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a75a80_TEST FAIL\n");
    return 1;
}