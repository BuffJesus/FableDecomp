#include <cstdio>

struct CCreatureActionBase { int vtbl; };

bool __fastcall CCreatureActionBase_IsStrikeAction(const CCreatureActionBase* self)
{
    (void)self;
    return false;
}

int main()
{
    CCreatureActionBase obj;
    obj.vtbl = 0x12345678;
    if (CCreatureActionBase_IsStrikeAction(&obj) == false) {
        std::printf("CCreatureActionBase_00692500_TEST PASS\n");
        return 0;
    }
    std::printf("CCreatureActionBase_00692500_TEST FAIL\n");
    return 1;
}