#include <cstdio>
void* __fastcall sub_0042931c(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0042931c(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0042931c_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0042931c_TEST FAIL\n");
    return 1;
}