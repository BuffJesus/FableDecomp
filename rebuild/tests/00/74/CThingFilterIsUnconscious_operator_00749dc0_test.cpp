#include <cstdio>
bool __fastcall fn_operator()
{
    return false;
}
int main()
{
    if (fn_operator() == false) { std::printf("AUTO_TINY_00749dc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00749dc0_TEST FAIL\n");
    return 1;
}