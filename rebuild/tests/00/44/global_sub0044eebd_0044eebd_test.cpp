#include <cstdio>
void* __fastcall sub_0044eebd(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0044eebd(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0044eebd_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0044eebd_TEST FAIL\n");
    return 1;
}