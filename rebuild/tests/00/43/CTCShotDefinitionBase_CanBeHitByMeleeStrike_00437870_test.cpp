#include <cstdio>

struct Inner { char pad[0x18]; bool flag; };
struct CTCShotDefinitionBase { char pad[0x160]; Inner* def; };

bool __fastcall CTCShotDefinitionBase_CanBeHitByMeleeStrike(const CTCShotDefinitionBase* self)
{
    return self->def->flag;
}

int main()
{
    Inner leaf;
    CTCShotDefinitionBase obj;
    obj.def = &leaf;
    leaf.flag = true;
    bool r1 = CTCShotDefinitionBase_CanBeHitByMeleeStrike(&obj);
    leaf.flag = false;
    bool r2 = CTCShotDefinitionBase_CanBeHitByMeleeStrike(&obj);
    if (r1 == true) { if (r2 == false) {
        std::printf("CTCShotDefinitionBase_00437870_TEST PASS\n");
        return 0;
    } }
    std::printf("FAIL\n");
    return 1;
}