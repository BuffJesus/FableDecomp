#include <cstdio>
void* __fastcall sub_0044eecc(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0044eecc(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0044eecc_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0044eecc_TEST FAIL\n");
    return 1;
}