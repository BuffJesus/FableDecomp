#include <cstdio>

struct Ring { int hit; };

static int g_reset_count = 0;
static int g_freed = 0;

void __fastcall Ring_reset(Ring* r) { r->hit = 1; g_reset_count++; }
void __cdecl engine_free(void* p) { g_freed++; }

struct CCombatWheel {
    Ring* begin;
    Ring* end;
};

void __fastcall CCombatWheel_ResetRings(CCombatWheel* self)
{
    Ring* s = self->begin;
    Ring* e = self->end;
    if (s != e) {
        do {
            Ring_reset(s);
            s = (Ring*)((char*)s + 4);
        } while (s != e);
    }
    void* base = self->begin;
    if (base) {
        engine_free(base);
    }
}

int main() {
    Ring arr[3];
    arr[0].hit = arr[1].hit = arr[2].hit = 0;
    CCombatWheel w;
    w.begin = arr;
    w.end = arr + 3;
    CCombatWheel_ResetRings(&w);
    if (g_reset_count != 3) { std::printf("FAIL reset_count=%d\n", g_reset_count); return 1; }
    if (arr[0].hit != 1 || arr[1].hit != 1 || arr[2].hit != 1) { std::printf("FAIL hit\n"); return 1; }
    if (g_freed != 1) { std::printf("FAIL freed=%d\n", g_freed); return 1; }

    // empty range: no resets, but begin non-null -> free called
    CCombatWheel w2;
    w2.begin = arr;
    w2.end = arr;
    g_reset_count = 0; g_freed = 0;
    CCombatWheel_ResetRings(&w2);
    if (g_reset_count != 0) { std::printf("FAIL empty reset\n"); return 1; }
    if (g_freed != 1) { std::printf("FAIL empty freed\n"); return 1; }

    // null begin: no free
    CCombatWheel w3;
    w3.begin = 0;
    w3.end = 0;
    g_reset_count = 0; g_freed = 0;
    CCombatWheel_ResetRings(&w3);
    if (g_reset_count != 0 || g_freed != 0) { std::printf("FAIL null\n"); return 1; }

    std::printf("CCombatWheel_00433040_TEST PASS\n");
    return 0;
}