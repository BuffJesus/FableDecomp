#include <cstdio>
int __fastcall CInputProcessInventoryClothing_GetPInventory(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 52);
}
int main()
{
    unsigned char object[56] = {0};
    *reinterpret_cast<int*>(object + 52) = 0x12345678;
    if (CInputProcessInventoryClothing_GetPInventory(object) == 0x12345678) { std::printf("AUTO_TINY_006896b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006896b0_TEST FAIL\n");
    return 1;
}