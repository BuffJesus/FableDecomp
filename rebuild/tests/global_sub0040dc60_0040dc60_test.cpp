#include <cstdio>
void* __fastcall sub_0040dc60(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0040dc60(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0040dc60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0040dc60_TEST FAIL\n");
    return 1;
}