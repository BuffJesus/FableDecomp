#include <cstdio>
void* __fastcall sub_0044eec2(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0044eec2(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0044eec2_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0044eec2_TEST FAIL\n");
    return 1;
}