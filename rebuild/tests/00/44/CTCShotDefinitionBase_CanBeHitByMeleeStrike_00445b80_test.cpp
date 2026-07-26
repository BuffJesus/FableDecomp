#include <cstdio>

struct CInner { char pad[0x48]; bool flag; };
struct CTCShotDefinitionBase { char pad[0x7a0]; CInner* inner; };

bool __fastcall CTCShotDefinitionBase_CanBeHitByMeleeStrike(const CTCShotDefinitionBase* self)
{
    return self->inner->flag;
}

int main()
{
    CInner innerObj;
    innerObj.flag = true;
    CTCShotDefinitionBase obj;
    obj.inner = &innerObj;
    bool r1 = CTCShotDefinitionBase_CanBeHitByMeleeStrike(&obj);
    innerObj.flag = false;
    bool r2 = CTCShotDefinitionBase_CanBeHitByMeleeStrike(&obj);
    if (r1 == true && r2 == false) {
        std::printf("CTCShotDefinitionBase_00445b80_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}