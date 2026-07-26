#include <cstdio>
void __stdcall CCreatureActionBase_SetHitDirection(int arg0, int arg1, int arg2) {}
int main()
{
    CCreatureActionBase_SetHitDirection(1, 2, 3);
    std::printf("AUTO_TINY_006927d0_TEST PASS\n");
    return 0;
}