#include <cstdio>

static void* g_captured = 0;

void __fastcall _Cons_val_tail_00f35a10(void* self) { g_captured = self; }

extern void __fastcall _Cons_val_pair_EHeroMorphType_CEntry_v2(void* self);

int main()
{
    // Fake object: byte buffer with a pointer stored at +0x2c
    char obj[0x40];
    void* member = (void*)0xDEADBEEF;
    *(void**)(obj + 0x2c) = member;

    _Cons_val_pair_EHeroMorphType_CEntry_v2(obj);

    if (g_captured == member)
        std::printf("00cb7920_TEST PASS\n");
    else
        std::printf("FAIL got=%p\n", g_captured);
    return 0;
}