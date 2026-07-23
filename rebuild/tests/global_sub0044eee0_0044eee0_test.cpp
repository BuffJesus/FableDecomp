#include <cstdio>
void* __fastcall sub_0044eee0(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0044eee0(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0044eee0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0044eee0_TEST FAIL\n");
    return 1;
}