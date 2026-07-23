#include <cstdio>
void* __fastcall sub_0044eed1(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0044eed1(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0044eed1_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0044eed1_TEST FAIL\n");
    return 1;
}