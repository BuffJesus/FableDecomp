#include <cstdio>
void* __fastcall sub_004292ec(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_004292ec(&x, 1, 2) == &x) { std::printf("AUTO_TINY_004292ec_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004292ec_TEST FAIL\n");
    return 1;
}