#include <cstdio>
void __stdcall CCreatureActionBase_SetHitPosition(int arg0, int arg1, int arg2) {}
int main()
{
    CCreatureActionBase_SetHitPosition(1, 2, 3);
    std::printf("AUTO_TINY_006927e0_TEST PASS\n");
    return 0;
}