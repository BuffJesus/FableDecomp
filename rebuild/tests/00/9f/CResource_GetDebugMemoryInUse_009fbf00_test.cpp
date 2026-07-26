#include <cstdio>
int __fastcall CResource_GetDebugMemoryInUse(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 20);
}
int main()
{
    unsigned char object[24] = {0};
    *reinterpret_cast<int*>(object + 20) = 0x12345678;
    if (CResource_GetDebugMemoryInUse(object) == 0x12345678) { std::printf("AUTO_TINY_009fbf00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009fbf00_TEST FAIL\n");
    return 1;
}