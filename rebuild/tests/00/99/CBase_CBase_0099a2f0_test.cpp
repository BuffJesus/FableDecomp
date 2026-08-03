#include <stdio.h>
#include <new>

class CBase {
public:
    CBase();
    virtual void Unused1() {}
};

CBase::CBase()
{
}

int main()
{
    char bufA[64];
    char bufB[64];
    int i;
    for (i = 0; i < 64; i++) bufA[i] = (char)0xCC;
    for (i = 0; i < 64; i++) bufB[i] = (char)0xCC;

    CBase* objA = (CBase*)bufA;
    void** vptrSlotA = (void**)bufA;
    void** vptrSlotB = (void**)bufB;

    CBase* retA = new (bufA) CBase();
    CBase* retB = new (bufB) CBase();

    void* installedVptrA = *vptrSlotA;
    void* installedVptrB = *vptrSlotB;

    if (retA == objA && installedVptrA != 0 && installedVptrA != (void*)0xCCCCCCCC
        && installedVptrA == installedVptrB) {
        printf("CBASE_CTOR_TEST_OK\n");
    } else {
        printf("CBASE_CTOR_TEST_FAIL retA=%p objA=%p vptrA=%p vptrB=%p\n",
            (void*)retA, (void*)objA, installedVptrA, installedVptrB);
    }

    return 0;
}