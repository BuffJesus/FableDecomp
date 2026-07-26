#include <cstdio>
void* __fastcall sub_004292fb(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_004292fb(&x, 1, 2) == &x) { std::printf("AUTO_TINY_004292fb_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004292fb_TEST FAIL\n");
    return 1;
}