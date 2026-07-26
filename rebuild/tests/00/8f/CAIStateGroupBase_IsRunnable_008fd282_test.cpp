#include <cstdio>
bool __fastcall CAIStateGroupBase_IsRunnable()
{
    return true;
}
int main()
{
    if (CAIStateGroupBase_IsRunnable() == true) { std::printf("AUTO_TINY_008fd282_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fd282_TEST FAIL\n");
    return 1;
}