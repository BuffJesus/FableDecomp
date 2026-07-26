#include <cstdio>
void* __fastcall sub_004039f0(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_004039f0(&x, 1, 2) == &x) { std::printf("AUTO_TINY_004039f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004039f0_TEST FAIL\n");
    return 1;
}