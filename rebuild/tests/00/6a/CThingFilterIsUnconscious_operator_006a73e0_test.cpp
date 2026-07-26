#include <cstdio>
bool __fastcall fn_operator(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!fn_operator(&object, 1, 2)) { std::printf("AUTO_TINY_006a73e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a73e0_TEST FAIL\n");
    return 1;
}