#include <cstdio>
void* __fastcall sub_00428eb2(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_00428eb2(&x, 1, 2) == &x) { std::printf("AUTO_TINY_00428eb2_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00428eb2_TEST FAIL\n");
    return 1;
}