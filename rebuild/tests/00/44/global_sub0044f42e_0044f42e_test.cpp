#include <cstdio>
void* __fastcall sub_0044f42e(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (sub_0044f42e(&x, 1, 2) == &x) { std::printf("AUTO_TINY_0044f42e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0044f42e_TEST FAIL\n");
    return 1;
}