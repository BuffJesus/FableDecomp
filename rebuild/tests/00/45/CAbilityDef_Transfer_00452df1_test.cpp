#include <cstdio>

struct CAbilityDef { char pad[0x28]; int marker; };

static void* g_passed = 0;
struct CTransferData {
    void* got;
    void Accept(void* p) { got = p; g_passed = p; }
};

void __fastcall CAbilityDef_Transfer(CAbilityDef* self, void* edx, CTransferData* t)
{
    (void)edx;
    t->Accept((char*)self + 0x28);
}

int main()
{
    CAbilityDef def;
    CTransferData td;
    td.got = 0;
    CAbilityDef_Transfer(&def, 0, &td);
    if (td.got == (char*)&def + 0x28 && g_passed == (char*)&def + 0x28) {
        std::printf("CAbilityDef_00452df1_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}