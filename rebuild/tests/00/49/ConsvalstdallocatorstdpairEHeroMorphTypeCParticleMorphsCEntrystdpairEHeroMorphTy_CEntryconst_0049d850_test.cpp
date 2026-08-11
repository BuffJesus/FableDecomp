#include <cstdio>

static void* g_seen = 0;

void __fastcall sub_4498c0(void* p) { g_seen = p; }

void __fastcall _Cons_val_pair_EHeroMorphType_CEntry(void* self);

int main()
{
    struct Obj { void* pad0; void* pad4; void* pad8; void* fc; } o;
    void* marker = (void*)0xDEADBEEF;
    o.fc = marker;
    _Cons_val_pair_EHeroMorphType_CEntry(&o);
    if (g_seen == marker)
        std::printf("0049d850_TEST PASS\n");
    else
        std::printf("FAIL got=%p\n", g_seen);
    return 0;
}