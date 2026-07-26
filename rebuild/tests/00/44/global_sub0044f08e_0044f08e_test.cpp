#include <cstdio>
void* __fastcall sub_0044f08e(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0044f08e(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0044f08e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0044f08e_TEST FAIL\n");
    return 1;
}