#include <cstdio>
void* __fastcall sub_0043cf60(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0043cf60(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0043cf60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0043cf60_TEST FAIL\n");
    return 1;
}