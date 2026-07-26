#include <cstdio>

struct CCreatureActionBase { int _vfptr; };

bool __fastcall CCreatureActionBase_IsStrikeEventActive(const CCreatureActionBase* self);

int main()
{
    CCreatureActionBase obj;
    obj._vfptr = 0;
    if (CCreatureActionBase_IsStrikeEventActive(&obj) == false) {
        std::printf("CCreatureActionBase_006924f0_TEST PASS\n");
        return 0;
    }
    std::printf("CCreatureActionBase_006924f0_TEST FAIL\n");
    return 1;
}
