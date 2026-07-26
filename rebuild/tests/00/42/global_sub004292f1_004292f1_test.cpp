#include <cstdio>
void* __fastcall sub_004292f1(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_004292f1(&x, 1, 2) == &x) { std::printf("AUTO_TINY_004292f1_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004292f1_TEST FAIL\n");
    return 1;
}