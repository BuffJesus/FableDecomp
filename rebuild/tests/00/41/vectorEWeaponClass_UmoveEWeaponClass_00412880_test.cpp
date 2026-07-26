#include <cstdio>

struct EWeaponClass { int x; };

struct AllocT {
    void* seen_self;
    EWeaponClass* seen_p;
    char* seen_local;
    EWeaponClass* construct(EWeaponClass* p, char* local);
};

EWeaponClass* AllocT::construct(EWeaponClass* p, char* local)
{
    seen_self = this;
    seen_p = p;
    seen_local = local;
    return p;
}

EWeaponClass* __fastcall vector_Umove(AllocT* self, int edx_unused, EWeaponClass* p)
{
    char local = 0;
    return self->construct(p, &local);
}

int main()
{
    EWeaponClass w;
    w.x = 42;
    AllocT a;
    a.seen_self = 0; a.seen_p = 0; a.seen_local = 0;
    EWeaponClass* r = vector_Umove(&a, 0, &w);
    if (r != &w) { std::printf("FAIL: return mismatch\n"); return 1; }
    if (a.seen_self != &a) { std::printf("FAIL: self mismatch\n"); return 1; }
    if (a.seen_p != &w) { std::printf("FAIL: p mismatch\n"); return 1; }
    if (a.seen_local == 0) { std::printf("FAIL: local null\n"); return 1; }
    if (*a.seen_local != 0) { std::printf("FAIL: local not zeroed\n"); return 1; }
    std::printf("vector_00412880_TEST PASS\n");
    return 0;
}