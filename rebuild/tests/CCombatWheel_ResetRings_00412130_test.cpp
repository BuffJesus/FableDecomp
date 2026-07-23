#include <cstdio>

struct Ring { int v; };
static int g_dtor_calls = 0;
static int g_free_calls = 0;

extern "C" void __fastcall Ring_dtor(Ring* r) { g_dtor_calls++; r->v = -1; }
extern "C" void engine_free(void* p) { g_free_calls++; }

struct CCombatWheel {
    Ring* begin;
    Ring* end;
};

void __fastcall CCombatWheel_ResetRings(CCombatWheel* self)
{
    Ring* p = self->begin;
    Ring* e = self->end;
    if (p != e) {
        do {
            Ring_dtor(p);
            p++;
        } while (p != e);
    }
    Ring* b = self->begin;
    if (b) {
        engine_free(b);
    }
}

int main() {
    Ring arr[3];
    arr[0].v = 1; arr[1].v = 2; arr[2].v = 3;
    CCombatWheel w;
    w.begin = arr;
    w.end = arr + 3;
    CCombatWheel_ResetRings(&w);
    if (g_dtor_calls != 3) { std::printf("FAIL dtor=%d\n", g_dtor_calls); return 1; }
    if (g_free_calls != 1) { std::printf("FAIL free=%d\n", g_free_calls); return 1; }

    CCombatWheel empty;
    empty.begin = 0;
    empty.end = 0;
    g_dtor_calls = 0; g_free_calls = 0;
    CCombatWheel_ResetRings(&empty);
    if (g_dtor_calls != 0) { std::printf("FAIL empty dtor=%d\n", g_dtor_calls); return 1; }
    if (g_free_calls != 0) { std::printf("FAIL empty free=%d\n", g_free_calls); return 1; }

    std::printf("CCombatWheel_00412130_TEST PASS\n");
    return 0;
}