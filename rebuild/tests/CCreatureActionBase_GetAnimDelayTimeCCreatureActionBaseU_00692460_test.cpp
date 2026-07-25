#include <cstdio>

struct CCreatureActionBase {
    void* vtbl;
};

long __fastcall CCreatureActionBase_GetAnimDelayTime(const CCreatureActionBase* self);

int main()
{
    CCreatureActionBase obj;
    obj.vtbl = 0;
    long r = CCreatureActionBase_GetAnimDelayTime(&obj);
    if (r == 0) {
        std::printf("CCREATUREACTIONBASE_00692460_TEST PASS\n");
        return 0;
    }
    std::printf("CCREATUREACTIONBASE_00692460_TEST FAIL got=%ld\n", r);
    return 1;
}
