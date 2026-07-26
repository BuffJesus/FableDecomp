#include <cstdio>
#include <cstdlib>

struct Ring { int v; void Reset(); };
struct CCombatWheel {
    Ring* begin;
    Ring* end_;
};

static int g_reset_count = 0;
void Ring::Reset() { this->v = 0; ++g_reset_count; }

static int g_freed = 0;
void __cdecl ext_free(void* p) { g_freed = 1; std::free(p); }

void __fastcall CCombatWheel_ResetRings(CCombatWheel* self)
{
    Ring* p = self->begin;
    Ring* e = self->end_;
    while (p != e) {
        p->Reset();
        ++p;
    }
    Ring* base = self->begin;
    if (base) ext_free(base);
}

int main()
{
    Ring* arr = (Ring*)std::malloc(sizeof(Ring) * 3);
    arr[0].v = 5; arr[1].v = 6; arr[2].v = 7;
    CCombatWheel w;
    w.begin = arr;
    w.end_ = arr + 3;
    CCombatWheel_ResetRings(&w);
    if (g_reset_count != 3) { std::printf("FAIL count=%d\n", g_reset_count); return 1; }
    if (g_freed != 1) { std::printf("FAIL not freed\n"); return 1; }

    CCombatWheel w2;
    w2.begin = 0;
    w2.end_ = 0;
    g_reset_count = 0; g_freed = 0;
    CCombatWheel_ResetRings(&w2);
    if (g_reset_count != 0 || g_freed != 0) { std::printf("FAIL empty case\n"); return 1; }

    std::printf("CCombatWheel_00414ea0_TEST PASS\n");
    return 0;
}