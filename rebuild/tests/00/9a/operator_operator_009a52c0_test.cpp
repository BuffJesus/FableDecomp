#include <cstdio>
bool __fastcall fn_operator()
{
    return true;
}
int main()
{
    if (fn_operator() == true) { std::printf("AUTO_TINY_009a52c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009a52c0_TEST FAIL\n");
    return 1;
}