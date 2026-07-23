#include <cstdio>
void __fastcall sub_0041cd23(unsigned int* self)
{
    *self = 0x0122ffcc;
}
int main()
{
    unsigned int x = 0;
    sub_0041cd23(&x);
    if (x == 0x0122ffcc) { std::printf("AUTO_TINY_0041cd23_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0041cd23_TEST FAIL\n");
    return 1;
}