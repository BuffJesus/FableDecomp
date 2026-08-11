#include <cstdio>

struct SelfT {
    void* pad0;
    void* member;
};

static void* g_seen = 0;

extern void __fastcall _target_a75430(void* self);
void __fastcall _target_a75430(void* self) { g_seen = self; }

extern void __fastcall _Cons_val_pair_EHeroMorphType_CEntry(SelfT* self);

int main() {
    int slot = 0;
    void* expected = (void*)&slot;
    SelfT obj;
    void* pobj = (void*)&obj;
    obj.pad0 = (void*)0x11111111;
    obj.member = expected;
    _Cons_val_pair_EHeroMorphType_CEntry((SelfT*)pobj);
    if (g_seen == expected) {
        printf("00a2e710_TEST PASS\n");
    } else {
        printf("FAIL seen=%p exp=%p\n", g_seen, expected);
    }
    return 0;
}