#include <cstdio>
void* __fastcall sub_004292f6(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_004292f6(&x, 1, 2) == &x) { std::printf("AUTO_TINY_004292f6_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004292f6_TEST FAIL\n");
    return 1;
}